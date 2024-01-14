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
 * @brief  eCAL registration provider
 * 
 * All process internal publisher/subscriber, server/clients register here with all their attributes.
 * 
 * These information will be send cyclic (registration refresh) via UDP to external eCAL processes.
 * 
**/

#include <ecal/ecal_config.h>

#include "ecal_def.h"
#include "ecal_globals.h"
#include "ecal_registration_provider.h"

#include "io/udp/ecal_udp_configurations.h"
#include "io/udp/ecal_udp_sample_sender.h"

#include <chrono>

namespace eCAL
{
  std::atomic<bool> CRegistrationProvider::m_created;

  CRegistrationProvider::CRegistrationProvider() :
                    m_reg_refresh(CMN_REGISTRATION_REFRESH),
                    m_reg_topics(false),
                    m_reg_process(false)
  {
  }

  CRegistrationProvider::~CRegistrationProvider()
  {
    Destroy();
  }

  void CRegistrationProvider::Create(bool topics_, bool process_)
  {
    if(m_created) return;

    m_reg_refresh     = Config::GetRegistrationRefreshMs();

    m_reg_topics      = topics_;
    m_reg_process     = process_;

    // set network attributes
    IO::UDP::SSenderAttr attr;
    attr.address   = UDP::GetRegistrationAddress();
    attr.port      = UDP::GetRegistrationPort();
    attr.ttl       = UDP::GetMulticastTtl();
    attr.broadcast = UDP::IsBroadcast();
    attr.loopback  = true;
    attr.sndbuf    = Config::GetUdpMulticastSndBufSizeBytes();

    // create udp registration sender
    m_reg_sample_snd = std::make_shared<UDP::CSampleSender>(attr);

    // start cyclic registration thread
    m_reg_sample_snd_thread = std::make_shared<CCallbackThread>([this] { RegisterSendThread(); });
    m_reg_sample_snd_thread->start(std::chrono::milliseconds(Config::GetRegistrationRefreshMs()));

    m_created = true;
  }

  void CRegistrationProvider::Destroy()
  {
    if(!m_created) return;

    // stop cyclic registration thread
    m_reg_sample_snd_thread->stop();

    // send one last (un)registration message to the world
    // thank you and goodbye :-)
    UnregisterProcess();

    // destroy registration sample sender
    m_reg_sample_snd.reset();

    m_created = false;
  }

  bool CRegistrationProvider::RegisterTopic(const std::string& topic_name_, const std::string& topic_id_, const Registration::Sample& ecal_sample_, const bool force_)
  {
    if(!m_created)    return(false);
    if(!m_reg_topics) return(false);

    const std::lock_guard<std::mutex> lock(m_topics_map_sync);
    m_topics_map[topic_name_ + topic_id_] = ecal_sample_;
    if(force_)
    {
      RegisterProcess();
      // apply registration sample
      ApplySample(topic_name_, ecal_sample_);
    }

    return(true);
  }

  bool CRegistrationProvider::UnregisterTopic(const std::string& topic_name_, const std::string& topic_id_, const Registration::Sample& ecal_sample_, const bool force_)
  {
    if(!m_created) return(false);

    if (force_)
    {
      // apply unregistration sample
      ApplySample(topic_name_, ecal_sample_);
    }

    SampleMapT::iterator iter;
    const std::lock_guard<std::mutex> lock(m_topics_map_sync);
    iter = m_topics_map.find(topic_name_ + topic_id_);
    if(iter != m_topics_map.end())
    {
      m_topics_map.erase(iter);
      return(true);
    }

    return(false);
  }

  bool CRegistrationProvider::RegisterProcess()
  {
    if(!m_created)     return(false);
    if(!m_reg_process) return(false);

    Registration::Sample process_sample;
    process_sample.cmd_type = bct_reg_process;
    auto& process_sample_process = process_sample.process;
    process_sample_process.hname                = Process::GetHostName();
    process_sample_process.hgname               = Process::GetHostGroupName();
    process_sample_process.pid                  = Process::GetProcessID();
    process_sample_process.pname                = Process::GetProcessName();
    process_sample_process.uname                = Process::GetUnitName();
    process_sample_process.pparam               = Process::GetProcessParameter();
    process_sample_process.datawrite            = g_process_wbytes;
    process_sample_process.dataread             = g_process_rbytes;
    process_sample_process.state.severity       = static_cast<Registration::eProcessSeverity>(g_process_severity);
    process_sample_process.state.severity_level = static_cast<Registration::eProcessSeverityLevel>(g_process_severity_level);
    process_sample_process.state.info           = g_process_info;
#if ECAL_CORE_TIMEPLUGIN
    if (g_timegate() == nullptr)
    {
      process_sample_process.tsync_state = Registration::eTSyncState::tsync_none;
    }
    else
    {
      if (!g_timegate()->IsSynchronized())
      {
        process_sample_process.tsync_state = Registration::eTSyncState::tsync_none;
      }
      else
      {
        switch (g_timegate()->GetSyncMode())
        {
        case CTimeGate::eTimeSyncMode::realtime:
          process_sample_process.tsync_state = Registration::eTSyncState::tsync_realtime;
          break;
        case CTimeGate::eTimeSyncMode::replay:
          process_sample_process.tsync_state = Registration::eTSyncState::tsync_replay;
          break;
        default:
          process_sample_process.tsync_state = Registration::eTSyncState::tsync_none;
          break;
        }
      }
      process_sample_process.tsync_mod_name = g_timegate()->GetName();
    }
#endif

    // eCAL initialization state
    const unsigned int comp_state(g_globals()->GetComponents());
    process_sample_process.component_init_state = comp_state;
    std::string component_info;
    if ((comp_state & Init::Publisher)  != 0u) component_info += "|pub";
    if ((comp_state & Init::Subscriber) != 0u) component_info += "|sub";
    if ((comp_state & Init::Logging)    != 0u) component_info += "|log";
    if ((comp_state & Init::TimeSync)   != 0u) component_info += "|time";
    if (!component_info.empty()) component_info = component_info.substr(1);
    process_sample_process.component_init_info = component_info;

    process_sample_process.ecal_runtime_version = GetVersionString();

    // apply registration sample
    const bool return_value = ApplySample(Process::GetHostName(), process_sample);

    return return_value;
  }

  bool CRegistrationProvider::UnregisterProcess()
  {
	  if (!m_created)     return(false);
	  if (!m_reg_process) return(false);

	  Registration::Sample process_sample;
	  process_sample.cmd_type = bct_unreg_process;
	  auto& process_sample_process = process_sample.process;
	  process_sample_process.hname = Process::GetHostName();
	  process_sample_process.pid   = Process::GetProcessID();
	  process_sample_process.pname = Process::GetProcessName();
	  process_sample_process.uname = Process::GetUnitName();

    // apply unregistration sample
    const bool return_value = ApplySample(Process::GetHostName(), process_sample);

	  return return_value;
  }

  bool CRegistrationProvider::RegisterTopics()
  {
    if(!m_created)    return(false);
    if(!m_reg_topics) return(false);

    bool return_value {true};
    const std::lock_guard<std::mutex> lock(m_topics_map_sync);
    for(SampleMapT::const_iterator iter = m_topics_map.begin(); iter != m_topics_map.end(); ++iter)
    {
      //////////////////////////////////////////////
      // send sample to registration layer
      //////////////////////////////////////////////
      return_value &= ApplySample(iter->second.topic.tname, iter->second);
    }

    return return_value;
  }

  bool CRegistrationProvider::ApplySample(const std::string& sample_name_, const Registration::Sample& sample_)
  {
    if(!m_created) return(false);

    bool return_value {true};

    if (m_reg_sample_snd)
    {
      const std::lock_guard<std::mutex> lock(m_sample_buffer_sync);
      if (SerializeToBuffer(sample_, m_sample_buffer))
      {
        return_value &= (m_reg_sample_snd->Send(sample_name_, m_sample_buffer) != 0);
      }
    }

    return return_value;
  }

  void CRegistrationProvider::RegisterSendThread()
  {
    // calculate average receive bytes
    g_process_rbytes = static_cast<long long>(((double)g_process_rbytes_sum / m_reg_refresh) * 1000.0);
    g_process_rbytes_sum = 0;

    // calculate average write bytes
    g_process_wbytes = static_cast<long long>(((double)g_process_wbytes_sum / m_reg_refresh) * 1000.0);
    g_process_wbytes_sum = 0;

#if ECAL_CORE_SUBSCRIBER
    // refresh subscriber registration
    if (g_subgate() != nullptr) g_subgate()->RefreshRegistrations();
#endif

#if ECAL_CORE_PUBLISHER
    // refresh publisher registration
    if (g_pubgate() != nullptr) g_pubgate()->RefreshRegistrations();
#endif

    // register process
    RegisterProcess();

    // register topics
    RegisterTopics();
  }
}
