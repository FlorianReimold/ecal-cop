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

#include "ecal_sample_struct_registration.h"

#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"

namespace cereal
{
  // Serialization for eCAL::OSInfo
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::OSInfo& os_info)
  {
    archive
    (
      os_info.osname
    );
  }

  // Serialization for eCAL::Host
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::Host& host)
  {
    archive
    (
      host.hname,
      host.os
    );
  }

  // Serialization for eCAL::ProcessState
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::ProcessState& process_state)
  {
    archive
    (
      process_state.severity,
      process_state.severity_level,
      process_state.info
    );
  }

  // Serialization for eCAL::LayerParUdpMC
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::LayerParUdpMC& layer_par_udpmc)
  {
    // Add serialization code if LayerParUdpMC has members
    (void)archive;
    (void)layer_par_udpmc;
  }

  // Serialization for eCAL::ConnectionPar
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::ConnectionPar& connection_par)
  {
    archive
    (
      connection_par.layer_par_udpmc
    );
  }

  // Serialization for eCAL::TLayer
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::TLayer& tlayer)
  {
    archive
    (
      tlayer.type,
      tlayer.version,
      tlayer.confirmed,
      tlayer.par_layer
    );
  }

  // Serialization for eCAL::SProcess
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::Process& process)
  {
    archive
    (
      process.rclock,
      process.hname,
      process.hgname,
      process.pid,
      process.pname,
      process.uname,
      process.pparam,
      process.datawrite,
      process.dataread,
      process.state,
      process.tsync_state,
      process.tsync_mod_name,
      process.component_init_state,
      process.component_init_info,
      process.ecal_runtime_version
    );
  }

  // Serialization for eCAL::DataTypeInformation
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::DataTypeInformation& datatype_info)
  {
    archive
    (
      datatype_info.name,
      datatype_info.encoding,
      datatype_info.desc
    );
  }

  // Serialization for eCAL::Topic
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::Topic& topic)
  {
    archive
    (
      topic.rclock,
      topic.hname,
      topic.hgname,
      topic.pid,
      topic.pname,
      topic.uname,
      topic.tid,
      topic.tname,
      topic.direction,
      topic.ttype,
      topic.tdesc,
      topic.tdatatype,
      topic.tlayer,
      topic.tsize,
      topic.connections_loc,
      topic.connections_ext,
      topic.message_drops,
      topic.did, 
      topic.dclock, 
      topic.dfreq,
      topic.attr
    );
  }

  // Serialization for eCAL::Sample
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::Sample& sample)
  {
    archive
    (
      sample.cmd_type,
      sample.host,
      sample.process,
      sample.topic
    );
  }
}

namespace eCAL
{
  std::string SerializeToBinaryString(const Registration::Sample& registration_sample_)
  {
    std::ostringstream oss;
    cereal::PortableBinaryOutputArchive archive(oss);
    archive(registration_sample_);
    return oss.str();
  };

  bool DeserializeFromBuffer(const char* data_, size_t size_, Registration::Sample& registration_sample_)
  {
    std::istringstream iss(std::string(data_, size_));
    cereal::PortableBinaryInputArchive archive(iss);
    archive(registration_sample_);
    return true;
  };
}
