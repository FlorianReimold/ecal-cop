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
  // eCAL::Payload::TLayer
  template <class Archive>
  void serialize(Archive& archive, eCAL::Payload::TLayer& tlayer)
  {
    archive
    (
      tlayer.type,                      // transport layer type
      tlayer.version,                   // transport layer version
      tlayer.confirmed                  // transport layer used ?
    );
  }

  // eCAL::Payload::Topic
  template <class Archive>
  void serialize(Archive& archive, eCAL::Payload::Topic& topic)
  {
    archive
    (
      topic.hname,                      // host name
      topic.tid,                        // topic id
      topic.tname,                      // topic name
      topic.tlayer                      // active topic transport layers and it's specific parameter
    );
  }

  // eCAL::Payload::Content
  template <class Archive>
  void serialize(Archive& archive, eCAL::Payload::Content& content)
  {
    archive
    (
      content.id,                       // sample id  
      content.clock,                    // internal used clock  
      content.time,                     // time the content was updated  
      content.hash,                     // unique hash for that sample
      content.payload                   // octet stream
    );
  }

  // eCAL::Payload::Sample
  template <class Archive>
  void serialize(Archive& archive, eCAL::Payload::Sample& sample)
  {
    archive
    (
      sample.cmd_type,                  // sample command type
      sample.topic,                     // topic information
      sample.content                    // topic content
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
