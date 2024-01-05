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

#pragma once

#include "io/udp/ecal_udp_sample_sender.h"
#include "util/ecal_thread.h"
#include "serialization/ecal_sample.h"

#include <ecal/ecal_types.h>

#include <atomic>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace eCAL
{
  class CRegistrationProvider
  {
  public:
    CRegistrationProvider();
    ~CRegistrationProvider();

    void Create(bool topics_, bool process_);
    void Destroy();

    bool RegisterTopic(const std::string& topic_name_, const std::string& topic_id_, const eCAL::Registration::Sample& ecal_sample_, bool force_);
    bool UnregisterTopic(const std::string& topic_name_, const std::string& topic_id_, const eCAL::Registration::Sample& ecal_sample_, bool force_);

  protected:
    bool RegisterProcess();
    bool UnregisterProcess();
      
    bool RegisterTopics();

    bool ApplySample(const std::string& sample_name_, const eCAL::Registration::Sample& sample_);
      
    void RegisterSendThread();

    static std::atomic<bool>            m_created;
    int                                 m_reg_refresh;
    bool                                m_reg_topics;
    bool                                m_reg_process;

    std::shared_ptr<UDP::CSampleSender> m_reg_sample_snd;
    std::shared_ptr<CCallbackThread>    m_reg_sample_snd_thread;

    using SampleMapT = std::unordered_map<std::string, eCAL::Registration::Sample>;
    std::mutex                          m_topics_map_sync;
    SampleMapT                          m_topics_map;
  };
}
