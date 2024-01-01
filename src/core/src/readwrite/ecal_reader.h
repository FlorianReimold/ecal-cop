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
 * @brief  common eCAL data reader
**/

#pragma once

#include <ecal/ecal.h>
#include <ecal/ecal_callback.h>
#include <ecal/ecal_types.h>

#include "serialization/ecal_sample.h"
#include "util/ecal_expmap.h"

#include <condition_variable>
#include <mutex>
#include <atomic>
#include <set>
#include <queue>

#include <string>
#include <unordered_map>

namespace eCAL
{
  class CDataReader
  {
  public:
    CDataReader();
    ~CDataReader();

    static void InitializeLayers();

    bool Create(const std::string& topic_name_, const SDataTypeInformation& topic_info_);
    bool Destroy();

    bool Receive(std::string& buf_, long long* time_ = nullptr, int rcv_timeout_ms_ = 0);

    bool AddReceiveCallback(ReceiveCallbackT callback_);
    bool RemReceiveCallback();

    bool SetAttribute(const std::string& attr_name_, const std::string& attr_value_);
    bool ClearAttribute(const std::string& attr_name_);

    void SetID(const std::set<long long>& id_set_);

    void ApplyLocPublication(const std::string& process_id_, const std::string& tid_, const SDataTypeInformation& tinfo_);
    void RemoveLocPublication(const std::string& process_id_, const std::string& tid_);

    void ApplyExtPublication(const std::string& host_name_, const std::string& process_id_, const std::string& tid_, const SDataTypeInformation& tinfo_);
    void RemoveExtPublication(const std::string& host_name_, const std::string& process_id_, const std::string& tid_);

    std::string Dump(const std::string& indent_ = "");

    bool IsCreated() const {return(m_created);}

    std::string          GetTopicName()           const {return(m_topic_name);}
    std::string          GetTopicID()             const {return(m_topic_id);}
    SDataTypeInformation GetDataTypeInformation() const {return(m_topic_info);}

    void RefreshRegistration();

    size_t AddSample(const std::string& tid_, const char* payload_, size_t size_, long long id_, long long clock_, long long time_, size_t hash_, eCAL::eTLayerType layer_);

  protected:
    void SubscribeToLayers();
    void UnsubscribeFromLayers();

    bool Register(bool force_);
    bool Unregister();

    bool CheckMessageClock(const std::string& tid_, long long current_clock_);

    std::string                               m_host_name;
    std::string                               m_host_group_name;
    int                                       m_pid;
    std::string                               m_pname;
    std::string                               m_topic_name;
    std::string                               m_topic_id;
    SDataTypeInformation                      m_topic_info;
    std::map<std::string, std::string>        m_attr;
    std::atomic<size_t>                       m_topic_size;

    mutable std::mutex                        m_read_buf_mutex;
    std::condition_variable                   m_read_buf_cv;
    bool                                      m_read_buf_received;
    std::string                               m_read_buf;
    long long                                 m_read_time;

    std::mutex                                m_receive_callback_sync;
    ReceiveCallbackT                          m_receive_callback;

    std::deque<size_t>                        m_sample_hash_queue;

    std::atomic<long long>                    m_clock;
    long long                                 m_clock_old;
    std::chrono::steady_clock::time_point     m_rec_time;
    long                                      m_freq;

    std::set<long long>                       m_id_set;
    
    using WriterCounterMapT = std::unordered_map<std::string, long long>;
    WriterCounterMapT                         m_writer_counter_map;
    long long                                 m_message_drops;

    bool                                      m_use_ttype;
    bool                                      m_use_tdesc;

    bool                                      m_use_udp_mc_confirmed;

    std::atomic<bool>                         m_created;
  };
}
