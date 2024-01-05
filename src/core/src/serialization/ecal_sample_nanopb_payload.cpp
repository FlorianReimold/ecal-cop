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
 * @file   ecal_sample_nanopb_payload.cpp
 * @brief  eCAL payload sample (de)serialization - nanopb variant
**/

#include "ecal_sample_struct_payload.h"
#include "ecal_sample_nanopb_common.h"

#include "nanopb/ecal.pb.h"
#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"

namespace
{
  bool PayloadStruct2Buffer(const eCAL::Payload::Sample& source_sample_, std::vector<char>& target_buffer_)
  {
    eCAL_pb_Sample pb_sample = eCAL_pb_Sample_init_default;

    // command type
    pb_sample.cmd_type = static_cast<eCAL_pb_eCmdType>(source_sample_.cmd_type);

    // topic information
    pb_sample.has_topic = true;
    // hname
    eCAL::nanopb::encode_string(pb_sample.topic.hname, source_sample_.topic.hname);
    // tid
    eCAL::nanopb::encode_string(pb_sample.topic.tid, source_sample_.topic.tid);
    // tname
    eCAL::nanopb::encode_string(pb_sample.topic.tname, source_sample_.topic.tname);
    // tlayer
    eCAL::nanopb::encode_payload_layer(pb_sample.topic.tlayer, source_sample_.topic.tlayer);

    // topic content
    pb_sample.has_content = true;
    pb_sample.content.id    = source_sample_.content.id;
    pb_sample.content.clock = source_sample_.content.clock;
    pb_sample.content.time  = source_sample_.content.time;
    pb_sample.content.hash  = source_sample_.content.hash;

    // extract payload
    const char* payload_addr = nullptr;
    size_t      payload_size = 0;
    switch (source_sample_.content.payload.type)
    {
    case eCAL::Payload::pl_raw:
      payload_addr = source_sample_.content.payload.raw_addr;
      payload_size = source_sample_.content.payload.raw_size;
      break;
    case eCAL::Payload::pl_vec:
      payload_addr = source_sample_.content.payload.vec.data();
      payload_size = source_sample_.content.payload.vec.size();
      break;
    default:
      break;
    }

    // topic content payload
    eCAL::nanopb::SNanoBytes payload;
    payload.content = (pb_byte_t*)(payload_addr);
    payload.length  = payload_size;
    eCAL::nanopb::encode_bytes(pb_sample.content.payload, payload);

    ///////////////////////////////////////////////
    // evaluate byte size
    ///////////////////////////////////////////////
    pb_ostream_t pb_sizestream = { nullptr };
    pb_encode(&pb_sizestream, eCAL_pb_Sample_fields, &pb_sample);

    ///////////////////////////////////////////////
    // encode it
    ///////////////////////////////////////////////
    target_buffer_.resize(pb_sizestream.bytes_written);
    pb_ostream_t pb_ostream;
    pb_ostream = pb_ostream_from_buffer((pb_byte_t*)(target_buffer_.data()), target_buffer_.size());
    if (pb_encode(&pb_ostream, eCAL_pb_Sample_fields, &pb_sample))
    {
      return true;
    }
    
    return false;
  }

  bool Buffer2PayloadStruct(const char* data_, size_t size_, eCAL::Payload::Sample& target_sample_)
  {
    if (data_ == nullptr) return false;
    if (size_ == 0)       return false;

    // initialize
    eCAL_pb_Sample pb_sample = eCAL_pb_Sample_init_default;

    ///////////////////////////////////////////////
    // assign decoder
    ///////////////////////////////////////////////
    // hname
    eCAL::nanopb::decode_string(pb_sample.topic.hname, target_sample_.topic.hname);
    // tid
    eCAL::nanopb::decode_string(pb_sample.topic.tid, target_sample_.topic.tid);
    // tname
    eCAL::nanopb::decode_string(pb_sample.topic.tname, target_sample_.topic.tname);
    // tlayer
    eCAL::nanopb::decode_payload_layer(pb_sample.topic.tlayer, target_sample_.topic.tlayer);
    // topic content payload
    target_sample_.content.payload.type = eCAL::Payload::pl_vec;
    eCAL::nanopb::decode_bytes(pb_sample.content.payload, target_sample_.content.payload.vec);

    ///////////////////////////////////////////////
    // decode it
    ///////////////////////////////////////////////
    pb_istream_t pb_istream;
    pb_istream = pb_istream_from_buffer((pb_byte_t*)data_, size_);
    pb_decode(&pb_istream, eCAL_pb_Sample_fields, &pb_sample);

    ///////////////////////////////////////////////
    // assign values
    ///////////////////////////////////////////////
    // command type
    target_sample_.cmd_type = static_cast<eCAL::eCmdType>(pb_sample.cmd_type);

    // topic content
    target_sample_.content.id    = pb_sample.content.id;
    target_sample_.content.clock = pb_sample.content.clock;
    target_sample_.content.time  = pb_sample.content.time;
    target_sample_.content.hash  = pb_sample.content.hash;

    return true;
  }
}

namespace eCAL
{
  bool SerializeToBuffer(const Payload::Sample& source_sample_, std::vector<char>& target_buffer_)
  {
    target_buffer_.clear();
    return PayloadStruct2Buffer(source_sample_, target_buffer_);
  }

  bool DeserializeFromBuffer(const char* data_, size_t size_, Payload::Sample& target_sample_)
  {
    return Buffer2PayloadStruct(data_, size_, target_sample_);
  }
}
