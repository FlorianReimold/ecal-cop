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

#include "ecal_sample.h"

#include "cereal/archives/binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/tuple.hpp"
#include "cereal/types/utility.hpp"

#include <string>

namespace cereal
{
  // Serialization for eCAL::OSInfo
  template <class Archive>
  void serialize(Archive& archive, eCAL::OSInfo& os_info)
  {
    archive(os_info.osname);
  }

  // Serialization for eCAL::Host
  template <class Archive>
  void serialize(Archive& archive, eCAL::Host& host)
  {
    archive(host.hname, host.os);
  }

  // Serialization for eCAL::ProcessState
  template <class Archive>
  void serialize(Archive& archive, eCAL::ProcessState& process_state)
  {
    archive(process_state.severity, process_state.severity_level, process_state.info);
  }

  // Serialization for eCAL::LayerParUdpMC
  template <class Archive>
  void serialize(Archive& archive, eCAL::LayerParUdpMC& layer_par_udpmc)
  {
    // Add serialization code if LayerParUdpMC has members
    (void)archive;
    (void)layer_par_udpmc;
  }

  // Serialization for eCAL::ConnectionPar
  template <class Archive>
  void serialize(Archive& archive, eCAL::ConnectionPar& connection_par)
  {
    archive(connection_par.layer_par_udpmc);
  }

  // Serialization for eCAL::TLayer
  template <class Archive>
  void serialize(Archive& archive, eCAL::TLayer& tlayer)
  {
    archive(tlayer.type, tlayer.version, tlayer.confirmed, tlayer.par_layer);
  }

  // Serialization for eCAL::SProcess
  template <class Archive>
  void serialize(Archive& archive, eCAL::SProcess& sprocess)
  {
    archive(sprocess.rclock, sprocess.hname, sprocess.hgname, sprocess.pid, sprocess.pname,
      sprocess.uname, sprocess.pparam, sprocess.pmemory, sprocess.pcpu, sprocess.usrptime,
      sprocess.datawrite, sprocess.dataread, sprocess.state, sprocess.tsync_state,
      sprocess.tsync_mod_name, sprocess.component_init_state, sprocess.component_init_info,
      sprocess.ecal_runtime_version);
  }

  // Serialization for eCAL::DataTypeInformation
  template <class Archive>
  void serialize(Archive& archive, eCAL::DataTypeInformation& datatype_info)
  {
    archive(datatype_info.name, datatype_info.encoding, datatype_info.desc);
  }

  // Serialization for eCAL::Topic
  template <class Archive>
  void serialize(Archive& archive, eCAL::Topic& topic)
  {
    archive(topic.rclock, topic.hname, topic.hgname, topic.pid, topic.pname, topic.uname,
      topic.tid, topic.tname, topic.direction, topic.ttype, topic.tdesc, topic.tdatatype,
      topic.tlayer, topic.tsize, topic.connections_loc, topic.connections_ext,
      topic.message_drops, topic.did, topic.dclock, topic.dfreq, topic.attr);
  }

  // Serialization for eCAL::Content
  template <class Archive>
  void serialize(Archive& archive, eCAL::Content& content)
  {
    archive(content.id, content.clock, content.time, content.payload, content.hash);
  }

  // Serialization for eCAL::Sample
  template <class Archive>
  void serialize(Archive& archive, eCAL::Sample& sample)
  {
    archive(sample.cmd_type, sample.host, sample.process, sample.topic, sample.content);
  }
} // namespace cereal

namespace eCAL
{
  std::string SerializeToBinaryString(const Sample& source_sample_)
  {
    std::ostringstream oss;
    cereal::BinaryOutputArchive archive(oss);
    archive(source_sample_);
    return oss.str();
  };

  bool DeserializeFromBuffer(const char* data_, size_t size_, Sample& target_sample_)
  {
    std::istringstream iss(std::string(data_, size_));
    cereal::BinaryInputArchive archive(iss);
    archive(target_sample_);
    return true;
  };
}
