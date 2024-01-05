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
  std::string PayloadStruct2String(const eCAL::Payload::Sample& payload_)
  {
    eCAL_pb_Sample pb_sample = eCAL_pb_Sample_init_default;

    // command type
    pb_sample.cmd_type = static_cast<eCAL_pb_eCmdType>(payload_.cmd_type);

    // topic information
    pb_sample.has_topic = true;
    // hname
    eCAL::nanopb::encode_string(pb_sample.topic.hname, payload_.topic.hname);
    // tid
    eCAL::nanopb::encode_string(pb_sample.topic.tid, payload_.topic.tid);
    // tname
    eCAL::nanopb::encode_string(pb_sample.topic.tname, payload_.topic.tname);
    // tlayer
    eCAL::nanopb::encode_payload_layer(pb_sample.topic.tlayer, payload_.topic.tlayer);

    // topic content
    pb_sample.has_content = true;
    pb_sample.content.id    = payload_.content.id;
    pb_sample.content.clock = payload_.content.clock;
    pb_sample.content.time  = payload_.content.time;
    pb_sample.content.hash  = payload_.content.hash;

    // topic content payload
    eCAL::nanopb::SNanoBytes payload;
    payload.content = (pb_byte_t*)(payload_.content.payload.data());
    payload.length  = payload_.content.payload.size();
    eCAL::nanopb::encode_bytes(pb_sample.content.payload, payload);

    ///////////////////////////////////////////////
    // evaluate byte size
    ///////////////////////////////////////////////
    pb_ostream_t pb_sizestream = { 0 };
    pb_encode(&pb_sizestream, eCAL_pb_Sample_fields, &pb_sample);

    ///////////////////////////////////////////////
    // encode it
    ///////////////////////////////////////////////
    std::vector<pb_byte_t> pb_buffer;
    pb_buffer.resize(pb_sizestream.bytes_written);
    pb_ostream_t pb_ostream;
    pb_ostream = pb_ostream_from_buffer(pb_buffer.data(), pb_buffer.size());
    if (pb_encode(&pb_ostream, eCAL_pb_Sample_fields, &pb_sample))
    {
      return std::string((char*)pb_buffer.data(), pb_ostream.bytes_written);
    }
    
    return "";
  }

  bool Buffer2PayloadStruct(const char* data_, size_t size_, eCAL::Payload::Sample& payload_)
  {
    if (data_ == nullptr) return false;
    if (size_ == 0)       return false;

    // initialize
    eCAL_pb_Sample pb_sample = eCAL_pb_Sample_init_default;

    ///////////////////////////////////////////////
    // assign decoder
    ///////////////////////////////////////////////
    // hname
    eCAL::nanopb::decode_string(pb_sample.topic.hname, payload_.topic.hname);
    // tid
    eCAL::nanopb::decode_string(pb_sample.topic.tid, payload_.topic.tid);
    // tname
    eCAL::nanopb::decode_string(pb_sample.topic.tname, payload_.topic.tname);
    // tlayer
    eCAL::nanopb::decode_payload_layer(pb_sample.topic.tlayer, payload_.topic.tlayer);
    // topic content payload
    eCAL::nanopb::decode_bytes(pb_sample.content.payload, payload_.content.payload);

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
    payload_.cmd_type = static_cast<eCAL::eCmdType>(pb_sample.cmd_type);

    // topic content
    payload_.content.id    = pb_sample.content.id;
    payload_.content.clock = pb_sample.content.clock;
    payload_.content.time  = pb_sample.content.time;
    payload_.content.hash  = pb_sample.content.hash;

    return true;
  }
}

namespace eCAL
{
  std::string SerializeToBinaryString(const Payload::Sample& payload_sample_)
  {
    return PayloadStruct2String(payload_sample_);
  }

  bool DeserializeFromBuffer(const char* data_, size_t size_, Payload::Sample& payload_sample_)
  {
    return Buffer2PayloadStruct(data_, size_, payload_sample_);
  }
}
