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
 * @file   ecal_payload_protobuf.cpp
 * @brief  eCAL payload (de)serialization - protobuf variant
**/

#include "ecal_sample_struct_payload.h"

#ifdef _MSC_VER
  #pragma warning(push, 0) // disable proto warnings
#endif
#include <ecal/core/pb/ecal.pb.h>
#ifdef _MSC_VER
  #pragma warning(pop)
#endif

namespace
{
  eCAL::pb::Sample PayloadStruct2Proto(const eCAL::Payload::Sample& payload_)
  {
    eCAL::pb::Sample pb_payload;

    // payload command type
    pb_payload.set_cmd_type(static_cast<eCAL::pb::eCmdType>(payload_.cmd_type));

    // topic information
    eCAL::pb::Topic* pb_topic = pb_payload.mutable_topic();
    pb_topic->set_hname(payload_.topic.hname);
    pb_topic->set_tid(payload_.topic.tid);
    pb_topic->set_tname(payload_.topic.tname);
    for (const auto& layer : payload_.topic.tlayer)
    {
      eCAL::pb::TLayer* pb_layer = pb_topic->add_tlayer();
      pb_layer->set_type(static_cast<eCAL::pb::eTLayerType>(layer.type));
      pb_layer->set_version(layer.version);
      pb_layer->set_confirmed(layer.confirmed);
    }

    // topic content
    eCAL::pb::Content* pb_content = pb_payload.mutable_content();
    pb_content->set_id(payload_.content.id);
    pb_content->set_clock(payload_.content.clock);
    pb_content->set_time(payload_.content.time);
    pb_content->set_payload(payload_.content.payload.data(), payload_.content.payload.size());
    pb_content->set_hash(payload_.content.hash);

    return pb_payload;
  }

  eCAL::Payload::Sample Proto2PayloadStruct(const eCAL::pb::Sample& pb_payload_)
  {
    eCAL::Payload::Sample payload;

    // payload command type
    payload.cmd_type = static_cast<eCAL::eCmdType>(pb_payload_.cmd_type());

    // topic information
    const eCAL::pb::Topic& pb_topic = pb_payload_.topic();
    payload.topic.hname = pb_topic.hname();
    payload.topic.tid = pb_topic.tid();
    payload.topic.tname = pb_topic.tname();

    const auto& pb_tlayer_list = pb_topic.tlayer();
    payload.topic.tlayer.reserve(pb_tlayer_list.size());
    for (const auto& pb_tlayer : pb_tlayer_list)
    {
      eCAL::Payload::TLayer tlayer;
      tlayer.type = static_cast<eCAL::eTLayerType>(pb_tlayer.type());
      tlayer.version = pb_tlayer.version();
      tlayer.confirmed = pb_tlayer.confirmed();
      payload.topic.tlayer.push_back(tlayer);
    }

    // topic content
    const eCAL::pb::Content& pb_content = pb_payload_.content();
    payload.content.id = pb_content.id();
    payload.content.clock = pb_content.clock();
    payload.content.time = pb_content.time();
    const std::string& payload_str = pb_content.payload();
    payload.content.payload.assign(payload_str.begin(), payload_str.end());
    payload.content.hash = pb_content.hash();

    return payload;
  }
}

namespace eCAL
{
  std::string SerializeToBinaryString(const Payload::Sample& payload_sample_)
  {
    eCAL::pb::Sample pb_payload = PayloadStruct2Proto(payload_sample_);
    return pb_payload.SerializeAsString();
  }

  bool DeserializeFromBuffer(const char* data_, size_t size_, Payload::Sample& payload_sample_)
  {
    eCAL::pb::Sample pb_payload;
    if (pb_payload.ParseFromArray(data_, static_cast<int>(size_)))
    {
      payload_sample_ = Proto2PayloadStruct(pb_payload);
      return true;
    }
    return false;
  }
}
