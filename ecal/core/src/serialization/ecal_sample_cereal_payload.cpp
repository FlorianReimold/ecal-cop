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
 * @file   ecal_sample.cpp
 * @brief  eCAL sample (de)serialization - cereal variant
**/

#include "ecal_sample_struct_payload.h"

#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"

namespace cereal
{
  // Serialization for eCAL::TLayer
  template <class Archive>
  void serialize(Archive& archive, eCAL::Payload::TLayer& tlayer)
  {
    archive
    (
      tlayer.type,
      tlayer.version,
      tlayer.confirmed
    );
  }

  // Serialization for eCAL::Topic
  template <class Archive>
  void serialize(Archive& archive, eCAL::Payload::Topic& topic)
  {
    archive
    (
      topic.hname,
      topic.tid,
      topic.tname,
      topic.tlayer
    );
  }

  // Serialization for eCAL::Content
  template <class Archive>
  void serialize(Archive& archive, eCAL::Payload::Content& content)
  {
    archive
    (
      content.id,
      content.clock,
      content.time,
      content.hash,
      content.payload
    );
  }

  // Serialization for eCAL::Sample
  template <class Archive>
  void serialize(Archive& archive, eCAL::Payload::Sample& sample)
  {
    archive
    (
      sample.cmd_type,
      sample.topic,
      sample.content
    );
  }
}

namespace eCAL
{
  std::string SerializeToBinaryString(const Payload::Sample& payload_sample_)
  {
    std::ostringstream oss;
    cereal::PortableBinaryOutputArchive archive(oss);
    archive(payload_sample_);
    return oss.str();
  };

  bool DeserializeFromBuffer(const char* data_, size_t size_, Payload::Sample& payload_sample_)
  {
    std::istringstream iss(std::string(data_, size_));
    cereal::PortableBinaryInputArchive archive(iss);
    archive(payload_sample_);
    return true;
  };
}
