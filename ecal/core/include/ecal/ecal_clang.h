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
 * @file   ecal_clang.h
 * @brief  eCAL C language interface (to wrap ecal into other languages easily)
**/

#ifndef ECAL_CLANG_H_INCLUDED
#define ECAL_CLANG_H_INCLUDED

#include <ecal/ecalc.h>

/*************************************************************************/
/*  common                                                               */
/*************************************************************************/
/**
 * @brief  Get eCAL version string.
 *
 * @return  Full eCAL version string.
**/
ECAL_API const char* ecal_getversion();

/**
 * @brief  Get eCAL version date.
 *
 * @return  Full eCAL version date string.
**/
ECAL_API const char* ecal_getdate();

/**
 * @brief Initialize eCAL API.
 *
 * @param argc_        Number of command line arguments.
 * @param argv_        Array of command line arguments.
 * @param unit_name_   Defines the name of the eCAL unit.
 *
 * @return Zero if succeeded, 1 if already initialized, -1 if failed.
**/
ECAL_API int ecal_initialize(int argc_, char **argv_, const char* unit_name_);

/**
 * @brief Finalize eCAL API.
 *
 * @return Zero if succeeded, 1 if already initialized, -1 if failed.
**/
ECAL_API int ecal_finalize();

/**
 * @brief  Set process state info.
 *
 * @param severity_  Severity.
 * @param level_     Severity level.
 * @param info_      Info message.
 *
**/
ECAL_API void ecal_set_process_state(const int severity_, const int level_, const char* info_);

/**
 * @brief Return the eCAL process state.
 *
 * @return  True if eCAL is in proper state.
**/
ECAL_API bool ecal_ok();

/**
 * @brief  Free an eCAL memory block allocated by functions like
 *         mon_get_monitoring, mon_get_logging,
 *         sub_receive, that let eCAL allocate
 *         the memory internally.
**/
/**
 * @code
 *            // let eCAL allocate memory for the subscriber buffer and return the pointer to 'rcv_buf'
 *            const char* rcv_buf     = nullptr;
 *            int         rcv_buf_len = 0;
 *            long long   rcv_time    = 0;
 *            int         timeout     = 0;
 *            int ret = sub_receive(subscriber_handle, &rcv_buf, &rcv_buf_len, &rcv_time, timeout);
 *            if(rcv_buf_len > 0)
 *            {
 *              ...
 *              // PROCESS THE BUFFER CONTENT HERE
 *              ...
 *              // finally free the allocated memory
 *              ecal_free_mem((void*)rcv_buf);
 *            }
 * @endcode
**/
ECAL_API void ecal_free_mem(void* mem_);

/**
 * @brief  Sleep current thread.
 *
 * @param  time_ms_  Time to sleep in ms.
**/
ECAL_API void ecal_sleep_ms(const long time_ms_);

/**
 * @brief Enable eCAL message loop back,
 *          that means subscriber will receive messages from
 *          publishers of the same process (default == false).
 *
 * @param state_  Switch on message loop back..
**/
ECAL_API void ecal_enable_loopback(const int state_);

/*************************************************************************/
/*  logging                                                              */
/*************************************************************************/
/**
 * @brief Sets the log level.
 *
 * @param level_  The level.
**/
ECAL_API void log_setlevel(const int level_);

/**
 * @brief Log a message (with current log level).
 *
 * @param message_  The log message string.
**/
ECAL_API void log_message(const char* message_);

/*************************************************************************/
/*  publisher                                                            */
/*************************************************************************/
/**
 * @brief Create a publisher.
 *
 * @param topic_name_      Unique topic name.
 * @param topic_type_      Topic type name.
 *
 * @return  Handle of the created publisher or NULL if failed.
**/
ECAL_API ECAL_HANDLE pub_create(const char* topic_name_, const char* topic_type_);

/**
 * @brief Destroy a publisher. 
 *
 * @param handle_  Publisher handle. 
 *
 * @return  True if succeeded.
**/
ECAL_API bool pub_destroy(ECAL_HANDLE handle_);

/**
 * @brief Setup topic type name.
 *
 * @param handle_                  Publisher handle.
 * @param topic_type_name_         Topic type name.
 * @param topic_type_name_length_  Topic type name length.
 *
 * @return  True if succeeded.
**/
ECAL_API bool pub_set_type_name(ECAL_HANDLE handle_, const char* topic_type_name_, const int topic_type_name_length_);

/**
 * @brief Setup topic type description.
 *
 * @param handle_             Publisher handle.
 * @param topic_desc_         Topic type description.
 * @param topic_desc_length_  Topic type description length.
 *
 * @return  True if succeeded.
**/
ECAL_API bool pub_set_description(ECAL_HANDLE handle_, const char* topic_desc_, const int topic_desc_length_);

/**
 * @brief Set publisher maximum transmit bandwidth for the udp layer.
 *
 * @param handle_     Publisher handle.
 * @param bandwidth_  Maximum bandwidth in bytes/s (-1 == unlimited).
 *
 * @return  True if succeeded.
**/
ECAL_API bool pub_set_max_bandwidth_udp(ECAL_HANDLE handle_, long bandwidth_);

/**
 * @brief Send a message to all subscribers.
 *
 * @param handle_   Publisher handle.
 * @param payload_  Buffer that contains content to send.
 * @param length_   Send buffer length.
 * @param time_     Send time (-1 = use eCAL system time in us, default = -1).
 *
 * @return  Number of bytes sent.
**/
ECAL_API int pub_send(ECAL_HANDLE handle_, const char* payload_, const int length_, const long long time_);

/**
 * @brief Send a message to all subscribers synchronized with acknowledge timeout.
 *
 * This synchronized mode is currently implemented for local interprocess communication (shm-ecal layer) only.
 *
 * @param handle_                  Publisher handle.
 * @param payload_                 Buffer that contains content to send.
 * @param length_                  Length of buffer.
 * @param time_                    Send time (-1 = use eCAL system time in us).
 * @param acknowledge_timeout_ms_  Maximum time to wait for all subscribers acknowledge feedback in ms (content received and processed).
 *
 * @return  Number of bytes sent.
**/
ECAL_API int pub_send_sync(ECAL_HANDLE handle_, const char* payload_, const int length_, const long long time_, const long long acknowledge_timeout_ms_);

/*************************************************************************/
/*  subscriber                                                           */
/*************************************************************************/
/**
* @brief Create a subscriber.
 *
 * @param topic_name_      Unique topic name.
 * @param topic_type_      Topic type name.
 *
 * @return  Handle of the created subscriber or NULL if failed.
**/
ECAL_API ECAL_HANDLE sub_create(const char* topic_name_, const char* topic_type_);

/**
 * @brief Destroy a subscriber.
 *
 * @param handle_  Subscriber handle.
 *
 * @return  True if succeeded.
**/
ECAL_API bool sub_destroy(ECAL_HANDLE handle_);

/**
 * @brief Receive a message from the publisher.
 *
 * @param       handle_       Subscriber handle.
 * @param [out] rcv_buf_      Buffer to store the received message content.
 * @param [out] rcv_buf_len_  Length of allocated buffer,
 *                            eCAL is allocating the buffer for you, use ecal_free_mem to free the buffer finally.
 * @param [out] rcv_time_     Time from publisher in us.
 * @param       timeout_      Maximum time before receive operation returns (in milliseconds, -1 means infinite).
 *
 * @return  Length of received buffer.
**/
ECAL_API int sub_receive(ECAL_HANDLE handle_, const char** rcv_buf_, int* rcv_buf_len_, long long* rcv_time_, const int timeout_);

/**
 * @brief Receive a message from the publisher (able to process zero length buffer).
 *
 * @param       handle_       Subscriber handle.
 * @param [out] rcv_buf_      Buffer to store the received message content.
 * @param [out] rcv_buf_len_  Length of allocated buffer,
 *                            eCAL is allocating the buffer for you, use ecal_free_mem to free the buffer finally.
 * @param [out] rcv_time_     Time from publisher in us.
 * @param       timeout_      Maximum time before receive operation returns (in milliseconds, -1 means infinite).
 *
 * @return  True if succeeded.
**/
ECAL_API bool sub_receive_buffer(ECAL_HANDLE handle_, const char** rcv_buf_, int* rcv_buf_len_, long long* rcv_time_, const int timeout_);

/**
 * @brief Add callback function for incoming receives.
 *
 * @param handle_    Subscriber handle.
 * @param callback_  The callback function to add.
 * @param par_       User defined context that will be forwarded to the callback function.
 *
 * @return  True if succeeded.
**/
ECAL_API bool sub_add_receive_callback(ECAL_HANDLE handle_, const ReceiveCallbackCT callback_, void* par_);

/**
 * @brief Remove callback function for incoming receives.
 *
 * @param  handle_  Subscriber handle.
 *
 * @return  True if succeeded.
**/
ECAL_API bool sub_rem_receive_callback(ECAL_HANDLE handle_);

#endif /* ECAL_CLANG_H_INCLUDED */
