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
 * @brief  eCAL C language interface
**/

#include <ecal/ecal.h>
#include <ecal/ecal_clang.h>

#include <mutex>
#include <string>
#include <vector>
#include <functional>

static char* str_malloc(const std::string& buf_s_)
{
  void* cbuf = malloc(buf_s_.size());
  if(cbuf != nullptr)
  {
    memcpy(cbuf, buf_s_.data(), buf_s_.size());
  }
  return(static_cast<char*>(cbuf));
}

/****************************************/
/*      ecal_getversion                 */
/****************************************/
ECAL_API const char* ecal_getversion()
{
  return(ECAL_VERSION);
}

/****************************************/
/*      ecal_getdate                    */
/****************************************/
ECAL_API const char* ecal_getdate()
{
  return(ECAL_DATE);
}

/****************************************/
/*      ecal_initialize                 */
/****************************************/
ECAL_API int ecal_initialize(int argc_, char **argv_, const char* unit_name_)
{
  return(eCAL::Initialize(argc_, argv_, unit_name_));
}

/****************************************/
/*      ecal_finalize                   */
/****************************************/
ECAL_API int ecal_finalize()
{
  //* @return Zero if succeeded, 1 if still initialized, -1 if failed.
  return(eCAL::Finalize());
}

/****************************************/
/*      ecal_set_process_state          */
/****************************************/
ECAL_API void ecal_set_process_state(const int severity_, const int level_, const char* info_)
{
  return(eCAL::Process::SetState(eCAL_Process_eSeverity(severity_), eCAL_Process_eSeverity_Level(level_), info_));
}

/****************************************/
/*      ecal_ok                         */
/****************************************/
ECAL_API bool ecal_ok()
{
  return(eCAL::Ok());
}

/****************************************/
/*      ecal_free_mem                    */
/****************************************/
ECAL_API void ecal_free_mem(void* mem_)
{
  free(mem_);
}

/****************************************/
/*      ecal_sleep_ms                   */
/****************************************/
ECAL_API void ecal_sleep_ms(const long time_ms_)
{
  eCAL::Process::SleepMS(time_ms_);
}

/****************************************/
/*      ecal_enable_loopback            */
/****************************************/
ECAL_API void ecal_enable_loopback(const int state_)
{
  eCAL::Util::EnableLoopback(state_ != 0);
}

/****************************************/
/*      log_setlevel                    */
/****************************************/
ECAL_API void log_setlevel(const int level_)
{
  eCAL::Logging::SetLogLevel(eCAL_Logging_eLogLevel(level_));
}

/****************************************/
/*      log_message                     */
/****************************************/
ECAL_API void log_message(const char* message_)
{
  eCAL::Logging::Log(message_);
}


/****************************************/
/*      pub_create                      */
/****************************************/
ECAL_API ECAL_HANDLE pub_create(const char* topic_name_, const char* topic_type_)
{
#if ECAL_CORE_PUBLISHER
  eCAL::CPublisher* pub = new eCAL::CPublisher;
  if (!pub->Create(topic_name_, topic_type_))
  {
    delete pub;
    return(nullptr);
  }
  return(pub);
#else
  return(nullptr);
#endif
}

/****************************************/
/*      pub_destroy                     */
/****************************************/
ECAL_API bool pub_destroy(ECAL_HANDLE handle_)
{
#if ECAL_CORE_PUBLISHER
  eCAL::CPublisher* pub = static_cast<eCAL::CPublisher*>(handle_);
  if(pub != nullptr)
  {
    delete pub;
    pub = nullptr;
    return(true);
  }
  else
  {
    return(false);
  }
#else
  return(false);
#endif
}

/****************************************/
/*      pub_set_type_name              */
/****************************************/
ECAL_API bool pub_set_type_name(ECAL_HANDLE handle_, const char* topic_type_name_, const int topic_type_name_length_)
{
#if ECAL_CORE_PUBLISHER
  eCAL::CPublisher* pub = static_cast<eCAL::CPublisher*>(handle_);
  if (pub != nullptr)
  {
    return(pub->SetTypeName(std::string(topic_type_name_, static_cast<size_t>(topic_type_name_length_))));
  }
#endif
  return(false);
}

/****************************************/
/*      pub_set_description             */
/****************************************/
ECAL_API bool pub_set_description(ECAL_HANDLE handle_, const char* topic_desc_, const int topic_desc_length_)
{
#if ECAL_CORE_PUBLISHER
  eCAL::CPublisher* pub = static_cast<eCAL::CPublisher*>(handle_);
  if(pub != nullptr)
  {
    return(pub->SetDescription(std::string(topic_desc_, static_cast<size_t>(topic_desc_length_))));
  }
#endif
  return(false);
}

/****************************************/
/*      pub_set_max_bandwidth_udp       */
/****************************************/
ECAL_API bool pub_set_max_bandwidth_udp(ECAL_HANDLE handle_, long bandwidth_)
{
#if ECAL_CORE_PUBLISHER
  eCAL::CPublisher* pub = static_cast<eCAL::CPublisher*>(handle_);
  if (pub != nullptr)
  {
    return(pub->SetMaxBandwidthUDP(bandwidth_));
  }
#endif
  return(false);
}

/****************************************/
/*      pub_send                        */
/****************************************/
ECAL_API int pub_send(ECAL_HANDLE handle_, const char* payload_, const int length_, const long long time_)
{
#if ECAL_CORE_PUBLISHER
  eCAL::CPublisher* pub = static_cast<eCAL::CPublisher*>(handle_);
  if(pub != nullptr)
  {
    const size_t ret = pub->Send(payload_, static_cast<size_t>(length_), time_);
    if(static_cast<int>(ret) == length_)
    {
      return(length_);
    }
  }
#endif
  return(0);
}

/****************************************/
/*      sub_create                      */
/****************************************/
ECAL_API ECAL_HANDLE sub_create(const char* topic_name_, const char* topic_type_)
{
  eCAL::CSubscriber* sub = new eCAL::CSubscriber;
  if (!sub->Create(topic_name_, topic_type_))
  {
    delete sub;
    return(nullptr);
  }
  return(sub);
}

/****************************************/
/*      sub_destroy                     */
/****************************************/
ECAL_API bool sub_destroy(ECAL_HANDLE handle_)
{
  eCAL::CSubscriber* sub = static_cast<eCAL::CSubscriber*>(handle_);
  if(sub != nullptr)
  {
    delete sub;
    sub = nullptr;
    return(true);
  }
  return(false);
}

/****************************************/
/*      sub_receive                     */
/****************************************/
ECAL_API int sub_receive(ECAL_HANDLE handle_, const char** rcv_buf_, int* rcv_buf_len_, long long* rcv_time_, const int timeout_)
{
  eCAL::CSubscriber* sub = static_cast<eCAL::CSubscriber*>(handle_);
  if(sub != nullptr)
  {
    std::string rcv_buf;
    long long rcv_time  = 0;
    sub->ReceiveBuffer(rcv_buf, &rcv_time, timeout_);

    if(!rcv_buf.empty())
    {
      // this has to be freed by caller (ecal_free_mem)
      char* cbuf = str_malloc(rcv_buf);
      if(cbuf == nullptr) return(0);

      if (rcv_buf_ != nullptr) {
        *rcv_buf_ = cbuf;
        if (rcv_buf_len_ != nullptr) *rcv_buf_len_ = static_cast<int>(rcv_buf.size());
      }
      else {
        // free allocated memory:
        ecal_free_mem(cbuf);
        if (rcv_buf_len_ != nullptr) *rcv_buf_len_ = 0;
        // operation could't be completed successfully
        return(0);
      }

      if(rcv_time_ != nullptr)    *rcv_time_    = rcv_time;

      return(static_cast<int>(rcv_buf.size()));
    }
  }
  return(0);
}

/****************************************/
/*      sub_receive_buffer              */
/****************************************/
ECAL_API bool sub_receive_buffer(ECAL_HANDLE handle_, const char** rcv_buf_, int* rcv_buf_len_, long long* rcv_time_, const int timeout_)
{
  eCAL::CSubscriber* sub = static_cast<eCAL::CSubscriber*>(handle_);
  if (sub != nullptr)
  {
    std::string rcv_buf;
    long long rcv_time = 0;

    if (sub->ReceiveBuffer(rcv_buf, &rcv_time, timeout_))
    {
      // this has to be freed by caller (ecal_free_mem)
      char* cbuf = str_malloc(rcv_buf);
      if (cbuf == nullptr) return(false);

      if (rcv_buf_ != nullptr) {
        *rcv_buf_ = cbuf;
        if (rcv_buf_len_ != nullptr) *rcv_buf_len_ = static_cast<int>(rcv_buf.size());
      }
      else {
        // free allocated memory:
        ecal_free_mem(cbuf);
        if (rcv_buf_len_ != nullptr) *rcv_buf_len_ = 0;
        // operation couldn't be completed successfullly.
        return(false);
      }
      if (rcv_time_ != nullptr)    *rcv_time_ = rcv_time;

      return(true);
    }
  }
  return(false);
}

/****************************************/
/*      sub_add_receive_callback        */
/****************************************/
static std::mutex g_sub_receive_callback_mtx;
static void g_sub_receive_callback(const char* topic_name_, const struct eCAL::SReceiveCallbackData* data_, const ReceiveCallbackCT callback_, void* par_)
{
  const std::lock_guard<std::mutex> lock(g_sub_receive_callback_mtx);
  SReceiveCallbackDataC data;
  data.buf   = data_->buf;
  data.size  = data_->size;
  data.id    = data_->id;
  data.time  = data_->time;
  data.clock = data_->clock;
  callback_(topic_name_, &data, par_);
}

ECAL_API bool sub_add_receive_callback(ECAL_HANDLE handle_, const ReceiveCallbackCT callback_, void* par_)
{
  eCAL::CSubscriber* sub = static_cast<eCAL::CSubscriber*>(handle_);

  auto callback = std::bind(g_sub_receive_callback, std::placeholders::_1, std::placeholders::_2, callback_, par_);
  return(sub->AddReceiveCallback(callback));
}

/****************************************/
/*      sub_rem_receive_callback        */
/****************************************/
ECAL_API bool sub_rem_receive_callback(ECAL_HANDLE handle_)
{
  eCAL::CSubscriber* sub = static_cast<eCAL::CSubscriber*>(handle_);

  return(sub->RemReceiveCallback());
}
