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

#pragma once

#include <ecal/ecal_callback.h>
#include <ecal/ecal_payload_writer.h>
#include <ecal/ecal_types.h>

#include "ecal_def.h"
#include "util/ecal_expmap.h"

#include "udp/ecal_writer_udp_mc.h"

#include <mutex>
#include <string>
#include <atomic>
#include <map>
#include <vector>

namespace eCAL
{
  class CDataWriter
  {
  public:
    struct SExternalSubscriptionInfo
    {
      std::string host_name;
      std::string process_id;
      std::string topic_id;

      friend bool operator<(const SExternalSubscriptionInfo& l, const SExternalSubscriptionInfo& r)
      {
        return std::tie(l.host_name, l.process_id, l.topic_id)
          < std::tie(r.host_name, r.process_id, r.topic_id);
      }
    };

    struct SLocalSubscriptionInfo
    {
      std::string process_id;
      std::string topic_id;

      friend bool operator<(const SLocalSubscriptionInfo& l, const SLocalSubscriptionInfo& r)
      {
        return std::tie(l.process_id, l.topic_id)
          < std::tie(r.process_id, r.topic_id);
      }
    };

    CDataWriter();
    ~CDataWriter();

    bool Create(const std::string& topic_name_, const SDataTypeInformation& topic_info_);
    bool Destroy();

    bool SetDataTypeInformation(const SDataTypeInformation& topic_info_);

    bool SetAttribute(const std::string& attr_name_, const std::string& attr_value_);
    bool ClearAttribute(const std::string& attr_name_);

    void ShareType(bool state_);
    void ShareDescription(bool state_);

    bool SetMaxBandwidthUDP(long bandwidth_);

    size_t Write(CPayloadWriter& payload_, long long time_, long long id_);

    void ApplyLocSubscription(const SLocalSubscriptionInfo& local_info_, const SDataTypeInformation& tinfo_, const std::string& reader_par_);
    void RemoveLocSubscription(const SLocalSubscriptionInfo& local_info_);

    void ApplyExtSubscription(const SExternalSubscriptionInfo& external_info_, const SDataTypeInformation& tinfo_, const std::string& reader_par_);
    void RemoveExtSubscription(const SExternalSubscriptionInfo& external_info_);

    void RefreshRegistration();
    void RefreshSendCounter();

    std::string Dump(const std::string& indent_ = "");

    bool IsCreated() const {return(m_created);}
    bool IsSubscribed() const {return(m_loc_subscribed || m_ext_subscribed);}

    const std::string& GetTopicName() const {return(m_topic_name);}
    const SDataTypeInformation& GetDataTypeInformation() const { return m_topic_info; }

  protected:
    bool Register(bool force_);
    bool Unregister();

    size_t PrepareWrite(long long id_, size_t len_);

    std::string                           m_host_name;
    std::string                           m_host_group_name;
    int                                   m_pid;
    std::string                           m_pname;
    std::string                           m_topic_name;
    std::string                           m_topic_id;
    SDataTypeInformation                  m_topic_info;
    std::map<std::string, std::string>    m_attr;
    size_t                                m_topic_size;

    std::vector<char>                     m_payload_buffer;

    long long                             m_id;
    long long                             m_clock;
    long long                             m_clock_old;
    std::chrono::steady_clock::time_point m_snd_time;
    long                                  m_freq;

    long                                  m_bandwidth_max_udp;

    std::atomic<bool>                     m_loc_subscribed;
    std::atomic<bool>                     m_ext_subscribed;

    struct SWriter
    {
      struct SWriterMode
      {
        bool              activated = false;
        bool              confirmed = false;
      };

      SWriterMode        udp_mc_mode;
      CDataWriterUdpMC   udp_mc;
    };
    SWriter                               m_writer;

    bool                                  m_use_ttype;
    bool                                  m_use_tdesc;
    int                                   m_share_ttype;
    int                                   m_share_tdesc;
    bool                                  m_created;
  };
}
