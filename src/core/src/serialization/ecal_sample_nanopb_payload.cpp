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
    eCAL_pb_Sample pb_payload = eCAL_pb_Sample_init_default;

    // payload command type
    pb_payload.cmd_type = static_cast<eCAL_pb_eCmdType>(payload_.cmd_type);

    // topic information
    pb_payload.has_topic = true;
    eCAL_pb_Topic& pb_topic = pb_payload.topic;

    // hname
    pb_topic.hname.arg          = (void*)(payload_.topic.hname.c_str());
    pb_topic.hname.funcs.encode = &encode_string;

    // tid
    pb_topic.tid.arg = (void*)(payload_.topic.tid.c_str());
    pb_topic.tid.funcs.encode = &encode_string;

    // tname
    pb_topic.tname.arg = (void*)(payload_.topic.tname.c_str());
    pb_topic.tname.funcs.encode = &encode_string;

    // tlayer
    //pb_topic->tlayer.funcs.args   = ? ? ? ;
    //pb_topic->tlayer.funcs.encode = ? ? ? ;

    //for (size_t i = 0; i < payload_.topic.tlayer.size(); ++i)
    //{
      //pb_topic->tlayer[i].type = static_cast<eCAL_pb_eTLayerType>(payload_.topic.tlayer[i].type);
      //pb_topic->tlayer[i].version = payload_.topic.tlayer[i].version;
      //pb_topic->tlayer[i].confirmed = payload_.topic.tlayer[i].confirmed;
    //}

    // topic content
    pb_payload.has_content = true;
    eCAL_pb_Content& pb_content = pb_payload.content;
    pb_content.id    = payload_.content.id;
    pb_content.clock = payload_.content.clock;
    pb_content.time  = payload_.content.time;
    pb_content.hash  = payload_.content.hash;

    // topic payload
    SNanoBytes payload;
    payload.content = (pb_byte_t*)(payload_.content.payload.data());
    payload.length  = payload_.content.payload.size();
    pb_content.payload.arg          = &payload;
    pb_content.payload.funcs.encode = &encode_bytes;

    // evaluate byte size
    pb_ostream_t pb_sizestream = { 0 };
    pb_encode(&pb_sizestream, eCAL_pb_Sample_fields, &pb_payload);

    // encode it
    std::vector<pb_byte_t> pb_buffer;
    pb_buffer.resize(pb_sizestream.bytes_written);
    pb_ostream_t pb_ostream;
    pb_ostream = pb_ostream_from_buffer(pb_buffer.data(), pb_buffer.size());
    if (pb_encode(&pb_ostream, eCAL_pb_Sample_fields, &pb_payload))
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
    eCAL_pb_Sample pb_payload = eCAL_pb_Sample_init_default;

    // topic information
    eCAL_pb_Topic& pb_topic = pb_payload.topic;

    // hname
    pb_topic.hname.funcs.decode = &decode_string;
    pb_topic.hname.arg          = &payload_.topic.hname;

    // tid
    pb_topic.tid.funcs.decode = &decode_string;
    pb_topic.tid.arg = &payload_.topic.tid;

    // tname
    pb_topic.tname.funcs.decode = &decode_string;
    pb_topic.tname.arg = &payload_.topic.tname;

#if 0
    // tlayer
    //for (size_t i = 0; i < pb_topic->tlayer_count; ++i)
    //{
    //  eCAL::Payload::TLayer tlayer;
    //  tlayer.type = static_cast<eCAL::eTLayerType>(pb_topic->tlayer[i].type);
    //  tlayer.version = pb_topic->tlayer[i].version;
    //  tlayer.confirmed = pb_topic->tlayer[i].confirmed;
    //  payload.topic.tlayer.push_back(tlayer);
    //}
#endif

    // topic payload
    eCAL_pb_Content& pb_content = pb_payload.content;
    pb_content.payload.funcs.decode = &decode_payload;
    pb_content.payload.arg          = &payload_.content.payload;

    // decode it
    pb_istream_t pb_istream;
    pb_istream = pb_istream_from_buffer((pb_byte_t*)data_, size_);
    pb_decode(&pb_istream, eCAL_pb_Sample_fields, &pb_payload);

    // payload command type
    payload_.cmd_type = static_cast<eCAL::eCmdType>(pb_payload.cmd_type);

    // topic content
    payload_.content.id    = pb_content.id;
    payload_.content.clock = pb_content.clock;
    payload_.content.time  = pb_content.time;
    payload_.content.hash  = pb_content.hash;

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
