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
 * @file   ecal_callback_cimpl.h
 * @brief  eCAL callback interface events, structs and functions
**/

#ifndef ecal_callback_cimpl_h_included
#define ecal_callback_cimpl_h_included

/**
 * @brief eCAL common registration event type.
**/
enum eCAL_Registration_Event
{
  reg_event_none            = 0,
  reg_event_publisher       = 1,
  reg_event_subscriber      = 2,
  reg_event_process         = 4,
};

#ifdef _MSC_VER
#pragma pack(push, 8)
#endif

/**
 * @brief eCAL subscriber receive callback struct (C variant).
**/
struct SReceiveCallbackDataC
{
  void*     buf;    //!< payload buffer
  long      size;   //!< payload buffer size
  long long id;     //!< source id
  long long time;   //!< source time stamp
  long long clock;  //!< source write clock
};

#ifdef _MSC_VER
#pragma pack(pop)
#endif

/**
 * @brief eCAL receive callback function
 *
 * @param topic_name_  Topic name of the data source (publisher).
 * @param data_        Data payload struct.
 * @param par_         Forwarded user defined parameter.
**/
typedef void (*ReceiveCallbackCT)(const char* topic_name_, const struct SReceiveCallbackDataC* data_, void* par_);

#endif /* ecal_callback_cimpl_h_included */
