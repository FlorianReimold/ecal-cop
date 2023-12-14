/* ========================= eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2019 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= eCAL LICENSE =================================
*/

/**
 * @brief  common eCAL data writer
**/

#include <ecal/ecal.h>
#include <ecal/ecal_config.h>
#include <ecal/ecal_payload_writer.h>

#include "ecal_def.h"
#include "ecal_buffer_payload_writer.h"
#include "config/ecal_config_reader_hlp.h"

#if ECAL_CORE_REGISTRATION
#include "registration/ecal_registration_provider.h"
#endif

#include "ecal_writer.h"
#include "ecal_writer_base.h"
#include "ecal_process.h"

#include "pubsub/ecal_pubgate.h"

#include <sstream>
#include <chrono>
#include <functional>

struct SSndHash
{
  SSndHash(std::string t, long long c) : topic_id(t), snd_clock(c) {}
  std::string topic_id;
  long long   snd_clock;
};

namespace std
{
  template<>
  class hash<SSndHash> {
  public:
    size_t operator()(const SSndHash &h) const
    {
      const size_t h1 = std::hash<std::string>()(h.topic_id);
      const size_t h2 = std::hash<long long>()(h.snd_clock);
      return h1 ^ (h2 << 1);
    }
  };
}

namespace eCAL
{
  CDataWriter::CDataWriter() :
    m_host_name(Process::GetHostName()),
    m_host_group_name(Process::GetHostGroupName()),
    m_host_id(Process::internal::GetHostID()),
    m_pid(Process::GetProcessID()),
    m_pname(Process::GetProcessName()),
    m_topic_size(0),
    m_id(0),
    m_clock(0),
    m_clock_old(0),
    m_freq(0),
    m_bandwidth_max_udp(NET_BANDWIDTH_MAX_UDP),
    m_loc_subscribed(false),
    m_ext_subscribed(false),
    m_use_ttype(true),
    m_use_tdesc(true),
    m_share_ttype(-1),
    m_share_tdesc(-1),
    m_created(false)
  {
  }

  CDataWriter::~CDataWriter()
  {
    Destroy();
  }

  bool CDataWriter::Create(const std::string& topic_name_, const SDataTypeInformation& topic_info_)
  {
    if (m_created) return(false);

    // set defaults
    m_topic_name             = topic_name_;
    m_topic_id.clear();
    m_topic_info             = topic_info_;
    m_id                     = 0;
    m_clock                  = 0;
    m_clock_old              = 0;
    m_snd_time               = std::chrono::steady_clock::time_point();
    m_freq                   = 0;
    m_bandwidth_max_udp      = Config::GetMaxUdpBandwidthBytesPerSecond();
    m_ext_subscribed         = false;
    m_created                = false;

    // build topic id
    std::stringstream counter;
    counter << std::chrono::steady_clock::now().time_since_epoch().count();
    m_topic_id = counter.str();

    // allow to share topic type
    m_use_ttype = Config::IsTopicTypeSharingEnabled();

    // allow to share topic description
    m_use_tdesc = Config::IsTopicDescriptionSharingEnabled();

    // create udp writer and activate it
    if (m_writer.udp_mc.Create(m_host_name, m_topic_name, m_topic_id))
    {
#ifndef NDEBUG
      Logging::Log(log_level_debug4, m_topic_name + "::CDataWriter::Create::UDP_MC_WRITER - SUCCESS");
#endif
    }
    else
    {
      Logging::Log(log_level_error, m_topic_name + "::CDataWriter::Create::UDP_MC_WRITER - FAILED");
    }
    m_writer.udp_mc_mode.activated = true;

    // register
    Register(false);

    // mark as created
    m_created = true;

#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug1, m_topic_name + "::CDataWriter::Created");
#endif

    return(true);
  }

  bool CDataWriter::Destroy()
  {
    if (!m_created) return(false);

#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug1, m_topic_name + "::CDataWriter::Destroy");
#endif

    // destroy udp multicast writer
    m_writer.udp_mc.Destroy();

    // reset defaults
    m_id                     = 0;
    m_clock                  = 0;
    m_clock_old              = 0;
    m_snd_time               = std::chrono::steady_clock::time_point();
    m_freq                   = 0;
    m_bandwidth_max_udp      = Config::GetMaxUdpBandwidthBytesPerSecond();

    // unregister
    Unregister();

    m_created = false;

    return(true);
  }

  bool CDataWriter::SetDataTypeInformation(const SDataTypeInformation& topic_info_)
  {
    // Does it even make sense to register if the info is the same???
    const bool force = m_topic_info != topic_info_;
    m_topic_info = topic_info_;

#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug2, m_topic_name + "::CDataWriter::SetDescription");
#endif

    // register it
    Register(force);

    return(true);
  }

  bool CDataWriter::SetAttribute(const std::string &attr_name_, const std::string& attr_value_)
  {
    auto current_val = m_attr.find(attr_name_);

    const bool force = current_val == m_attr.end() || current_val->second != attr_value_;
    m_attr[attr_name_] = attr_value_;

#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug2, m_topic_name + "::CDataWriter::SetAttribute");
#endif

    // register it
    Register(force);

    return(true);
  }

  bool CDataWriter::ClearAttribute(const std::string& attr_name_)
  {
    auto force = m_attr.find(attr_name_) != m_attr.end();

    m_attr.erase(attr_name_);

#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug2, m_topic_name + "::CDataWriter::ClearAttribute");
#endif

    // register it
    Register(force);

    return(true);
  }

  void CDataWriter::ShareType(bool state_)
  {
    if (state_)
    {
      m_share_ttype = 1;
    }
    else
    {
      m_share_ttype = 0;
    }
  }

  void CDataWriter::ShareDescription(bool state_)
  {
    if (state_)
    {
      m_share_tdesc = 1;
    }
    else
    {
      m_share_tdesc = 0;
    }
  }

  bool CDataWriter::SetMaxBandwidthUDP(long bandwidth_)
  {
    m_bandwidth_max_udp = bandwidth_;
    return true;
  }

  size_t CDataWriter::Write(CPayloadWriter& payload_, long long time_, long long id_)
  {
    // get payload buffer size (one time, to avoid multiple computations)
    const size_t payload_buf_size(payload_.GetSize());

    // can we do a zero copy write ?
    const bool allow_zero_copy = false;

    // create a payload copy for all layer
    if (!allow_zero_copy)
    {
      m_payload_buffer.resize(payload_buf_size);
      payload_.WriteFull(m_payload_buffer.data(), m_payload_buffer.size());
    }

    // prepare counter and internal states
    const size_t snd_hash = PrepareWrite(id_, payload_buf_size);

    // did we write anything
    bool written(false);

    ////////////////////////////////////////////////////////////////////////////
    // LAYER 3 : UDP (MC)
    ////////////////////////////////////////////////////////////////////////////
    if (m_writer.udp_mc_mode.activated)
    {
#ifndef NDEBUG
      // log it
      Logging::Log(log_level_debug3, m_topic_name + "::CDataWriter::Send::UDP_MC");
#endif

      // send it
      bool udp_mc_sent(false);
      {
        // if shared memory layer for local communication is switched off
        // we activate udp message loopback to communicate with local processes too
        const bool loopback = true;

        // fill writer data
        struct SWriterAttr wattr;
        wattr.len       = payload_buf_size;
        wattr.id        = m_id;
        wattr.clock     = m_clock;
        wattr.hash      = snd_hash;
        wattr.time      = time_;
        wattr.bandwidth = m_bandwidth_max_udp;
        wattr.loopback  = loopback;

        // prepare send
        if (m_writer.udp_mc.PrepareWrite(wattr))
        {
          // register new to update listening subscribers and rematch
          Register(true);
          Process::SleepMS(5);
        }

        // write to udp multicast layer
        udp_mc_sent = m_writer.udp_mc.Write(m_payload_buffer.data(), wattr);
        m_writer.udp_mc_mode.confirmed = true;
      }
      written |= udp_mc_sent;

#ifndef NDEBUG
      // log it
      if (udp_mc_sent)
      {
        Logging::Log(log_level_debug3, m_topic_name + "::CDataWriter::Send::UDP_MC - SUCCESS");
      }
      else
      {
        Logging::Log(log_level_error, m_topic_name + "::CDataWriter::Send::UDP_MC - FAILED");
      }
#endif
    }

    // return success
    if (written) return payload_buf_size;
    else         return 0;
  }

  void CDataWriter::ApplyLocSubscription(const SLocalSubscriptionInfo& /*local_info_*/, const SDataTypeInformation& /*tinfo_*/, const std::string& /*reader_par_*/)
  {
    m_loc_subscribed = true;

#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug3, m_topic_name + "::CDataWriter::ApplyLocSubscription");
#endif
  }

  void CDataWriter::RemoveLocSubscription(const SLocalSubscriptionInfo& /*local_info_*/)
  {
  }

  void CDataWriter::ApplyExtSubscription(const SExternalSubscriptionInfo& /*external_info_*/, const SDataTypeInformation& /*tinfo_*/, const std::string& /*reader_par_*/)
  {
    m_ext_subscribed = true;

#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug3, m_topic_name + "::CDataWriter::ApplyExtSubscription");
#endif
  }

  void CDataWriter::RemoveExtSubscription(const SExternalSubscriptionInfo& /*external_info_*/)
  {
  }

  void CDataWriter::RefreshRegistration()
  {
    if (!m_created) return;

    // force to register every second to refresh data clock information
    auto curr_time = std::chrono::steady_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - m_snd_time) > std::chrono::milliseconds(0))
    {
      // reset clock and time on first call
      if (m_clock_old == 0)
      {
        m_clock_old = m_clock;
        m_snd_time = curr_time;
      }

      // check for clock difference
      if ((m_clock - m_clock_old) > 0)
      {
        // calculate frequency in mHz
        m_freq = static_cast<long>((1000 * 1000 * (m_clock - m_clock_old)) / std::chrono::duration_cast<std::chrono::milliseconds>(curr_time - m_snd_time).count());
        // reset clock and time
        m_clock_old = m_clock;
        m_snd_time  = curr_time;
      }
      else
      {
        m_freq = 0;
      }
    }

    // register without send
    Register(false);
  }

  void CDataWriter::RefreshSendCounter()
  {
    // increase write clock
    m_clock++;

    // statistics
    g_process_wclock++;
  }

  std::string CDataWriter::Dump(const std::string& indent_ /* = "" */)
  {
    std::stringstream out;

    out << std::endl;
    out << indent_ << "--------------------------"                            << std::endl;
    out << indent_ << " class CDataWriter  "                                  << std::endl;
    out << indent_ << "--------------------------"                            << std::endl;
    out << indent_ << "m_host_name:              " << m_host_name             << std::endl;
    out << indent_ << "m_host_group_name:        " << m_host_group_name       << std::endl;
    out << indent_ << "m_host_id:                " << m_host_id               << std::endl;
    out << indent_ << "m_topic_name:             " << m_topic_name            << std::endl;
    out << indent_ << "m_topic_id:               " << m_topic_id              << std::endl;
    out << indent_ << "m_topic_info.encoding:    " << m_topic_info.encoding   << std::endl;
    out << indent_ << "m_topic_info.name:        " << m_topic_info.name       << std::endl;
    out << indent_ << "m_topic_info.descriptor:  " << m_topic_info.descriptor << std::endl;
    out << indent_ << "m_id:                     " << m_id                    << std::endl;
    out << indent_ << "m_clock:                  " << m_clock                 << std::endl;
    out << indent_ << "m_created:                " << m_created               << std::endl;
    out << indent_ << "m_loc_subscribed:         " << m_loc_subscribed        << std::endl;
    out << indent_ << "m_ext_subscribed:         " << m_ext_subscribed        << std::endl;
    out << std::endl;

    return(out.str());
  }

  bool CDataWriter::Register(bool force_)
  {
#if ECAL_CORE_REGISTRATION
    if (m_topic_name.empty()) return(false);

    //@Rex: why is the logic different in CDataReader???
    // check share modes
    bool share_ttype(m_use_ttype && (g_pubgate() != nullptr) && g_pubgate()->TypeShared());
    if (m_share_ttype != -1)
    {
      share_ttype = m_share_ttype == 1;
    }
    bool share_tdesc(m_use_tdesc && (g_pubgate() != nullptr) && g_pubgate()->DescriptionShared());
    if (m_share_tdesc != -1)
    {
      share_tdesc = m_share_tdesc == 1;
    }

    // create command parameter
    eCAL::pb::Sample ecal_reg_sample;
    ecal_reg_sample.set_cmd_type(eCAL::pb::bct_reg_publisher);
    auto *ecal_reg_sample_mutable_topic = ecal_reg_sample.mutable_topic();
    ecal_reg_sample_mutable_topic->set_hname(m_host_name);
    ecal_reg_sample_mutable_topic->set_hgname(m_host_group_name);
    ecal_reg_sample_mutable_topic->set_hid(m_host_id);
    ecal_reg_sample_mutable_topic->set_tname(m_topic_name);
    ecal_reg_sample_mutable_topic->set_tid(m_topic_id);
    if (share_ttype) ecal_reg_sample_mutable_topic->set_ttype(Util::CombinedTopicEncodingAndType(m_topic_info.encoding, m_topic_info.name));
    if (share_tdesc) ecal_reg_sample_mutable_topic->set_tdesc(m_topic_info.descriptor);
    // topic_information
    {
      auto* ecal_reg_sample_mutable_tdatatype = ecal_reg_sample_mutable_topic->mutable_tdatatype();
      if (share_ttype)
      {
        ecal_reg_sample_mutable_tdatatype->set_encoding(m_topic_info.encoding);
        ecal_reg_sample_mutable_tdatatype->set_name(m_topic_info.name);
      }
      if (share_tdesc)
      {
        ecal_reg_sample_mutable_tdatatype->set_desc(m_topic_info.descriptor);
      }
    }
    *ecal_reg_sample_mutable_topic->mutable_attr() = google::protobuf::Map<std::string, std::string> { m_attr.begin(), m_attr.end() };
    ecal_reg_sample_mutable_topic->set_tsize(google::protobuf::int32(m_topic_size));
    // udp multicast layer
    {
      auto *udp_tlayer = ecal_reg_sample_mutable_topic->add_tlayer();
      udp_tlayer->set_type(eCAL::pb::tl_ecal_udp_mc);
      udp_tlayer->set_version(1);
      udp_tlayer->set_confirmed(m_writer.udp_mc_mode.confirmed);
      udp_tlayer->mutable_par_layer()->ParseFromString(m_writer.udp_mc.GetConnectionParameter());
    }
    ecal_reg_sample_mutable_topic->set_pid(m_pid);
    ecal_reg_sample_mutable_topic->set_pname(m_pname);
    ecal_reg_sample_mutable_topic->set_uname(Process::GetUnitName());
    ecal_reg_sample_mutable_topic->set_did(m_id);
    ecal_reg_sample_mutable_topic->set_dclock(m_clock);
    ecal_reg_sample_mutable_topic->set_dfreq(m_freq);

    // register publisher
    if (g_registration_provider() != nullptr) g_registration_provider()->RegisterTopic(m_topic_name, m_topic_id, ecal_reg_sample, force_);
#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug4, m_topic_name + "::CDataWriter::Register");
#endif

#endif // ECAL_CORE_REGISTRATION
    return(true);
  }

  bool CDataWriter::Unregister()
  {
#if ECAL_CORE_REGISTRATION
    if (m_topic_name.empty()) return(false);

    // create command parameter
    eCAL::pb::Sample ecal_unreg_sample;
    ecal_unreg_sample.set_cmd_type(eCAL::pb::bct_unreg_publisher);
    auto* ecal_reg_sample_mutable_topic = ecal_unreg_sample.mutable_topic();
    ecal_reg_sample_mutable_topic->set_hname(m_host_name);
    ecal_reg_sample_mutable_topic->set_hgname(m_host_group_name);
    ecal_reg_sample_mutable_topic->set_hid(m_host_id);
    ecal_reg_sample_mutable_topic->set_pname(m_pname);
    ecal_reg_sample_mutable_topic->set_pid(m_pid);
    ecal_reg_sample_mutable_topic->set_tname(m_topic_name);
    ecal_reg_sample_mutable_topic->set_tid(m_topic_id);
    ecal_reg_sample_mutable_topic->set_uname(Process::GetUnitName());

    // unregister publisher
    if (g_registration_provider() != nullptr) g_registration_provider()->UnregisterTopic(m_topic_name, m_topic_id, ecal_unreg_sample, true);
#ifndef NDEBUG
    // log it
    Logging::Log(log_level_debug4, m_topic_name + "::CDataWriter::UnRegister");
#endif

#endif // ECAL_CORE_REGISTRATION
    return(true);
  }

  size_t CDataWriter::PrepareWrite(long long id_, size_t len_)
  {
    // store id
    m_id = id_;

    // handle write counters
    RefreshSendCounter();

    // calculate unique send hash
    const std::hash<SSndHash> hf;
    const size_t snd_hash = hf(SSndHash(m_topic_id, m_clock));

    // increase overall sum send
    g_process_wbytes_sum += len_;

    // store size for monitoring
    m_topic_size = len_;

    // return the hash for the write action
    return snd_hash;
  }
}
