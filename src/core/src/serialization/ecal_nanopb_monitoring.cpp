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
 * @file   ecal_nanopb_monitoring.cpp
 * @brief  eCAL monitoring (de)serialization - nanopb variant
**/

#include "ecal_struct_monitoring.h"
#include "ecal_nanopb_common.h"

#include "nanopb/monitoring.pb.h"
#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"

#include <iostream>

namespace
{
  bool LogMessageStruct2Buffer(const eCAL::Logging::SLogMessage& log_message_, std::vector<char>& target_buffer_)
  {
    eCAL_pb_LogMessage pb_log_message = eCAL_pb_LogMessage_init_default;

    // time
    pb_log_message.time = log_message_.time;
    // hname
    eCAL::nanopb::encode_string(pb_log_message.hname, log_message_.hname);
    // pid
    pb_log_message.pid = log_message_.pid;
    // pname
    eCAL::nanopb::encode_string(pb_log_message.pname, log_message_.pname);
    // uname
    eCAL::nanopb::encode_string(pb_log_message.uname, log_message_.uname);
    // level
    pb_log_message.level = log_message_.level;
    // content
    eCAL::nanopb::encode_string(pb_log_message.content, log_message_.content);

    ///////////////////////////////////////////////
    // evaluate byte size
    ///////////////////////////////////////////////
    pb_ostream_t pb_sizestream = { nullptr };
    pb_encode(&pb_sizestream, eCAL_pb_LogMessage_fields, &pb_log_message);

    ///////////////////////////////////////////////
    // encode it
    ///////////////////////////////////////////////
    target_buffer_.resize(pb_sizestream.bytes_written);
    pb_ostream_t pb_ostream;
    pb_ostream = pb_ostream_from_buffer((pb_byte_t*)(target_buffer_.data()), target_buffer_.size());
    if (!pb_encode(&pb_ostream, eCAL_pb_LogMessage_fields, &pb_log_message))
    {
      std::cerr << "NanoPb eCAL::Logging::SLogMessage encode failed: " << pb_ostream.errmsg << std::endl;
    }
    else
    {
      return true;
    }

    return false;
  }

  bool Buffer2LogMessageStruct(const char* data_, size_t size_, eCAL::Logging::SLogMessage& log_message_)
  {
    if (data_ == nullptr) return false;
    if (size_ == 0)       return false;

    // initialize
    eCAL_pb_LogMessage pb_log_message = eCAL_pb_LogMessage_init_default;

    ///////////////////////////////////////////////
    // assign decoder
    ///////////////////////////////////////////////

    // hname
    eCAL::nanopb::decode_string(pb_log_message.hname, log_message_.hname);
    // pname
    eCAL::nanopb::decode_string(pb_log_message.pname, log_message_.pname);
    // uname
    eCAL::nanopb::decode_string(pb_log_message.uname, log_message_.uname);
    // content
    eCAL::nanopb::decode_string(pb_log_message.content, log_message_.content);

    ///////////////////////////////////////////////
    // decode it
    ///////////////////////////////////////////////
    pb_istream_t pb_istream;
    pb_istream = pb_istream_from_buffer((pb_byte_t*)data_, size_);
    if (!pb_decode(&pb_istream, eCAL_pb_LogMessage_fields, &pb_log_message))
    {
      std::cerr << "NanoPb eCAL::Logging::SLogMessage decode failed: " << pb_istream.errmsg << std::endl;
    }

    ///////////////////////////////////////////////
    // assign values
    ///////////////////////////////////////////////

    // time
    log_message_.time = pb_log_message.time;
    // pid
    log_message_.pid = pb_log_message.pid;
    // level
    log_message_.level = static_cast<eCAL_Logging_eLogLevel>(pb_log_message.level);

    return true;
  }
}

namespace eCAL
{
  // log message - serialize/deserialize
  bool SerializeToBuffer(const Logging::SLogMessage& source_sample_, std::vector<char>& target_buffer_)
  {
    target_buffer_.clear();
    return LogMessageStruct2Buffer(source_sample_, target_buffer_);
  }
  bool DeserializeFromBuffer(const char* data_, size_t size_, Logging::SLogMessage& target_sample_)
  {
    return Buffer2LogMessageStruct(data_, size_, target_sample_);
  }
}
