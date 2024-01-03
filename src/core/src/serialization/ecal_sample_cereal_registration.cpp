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
 * @file   ecal_sample_cereal_registration.cpp
 * @brief  eCAL registration sample (de)serialization - cereal variant
**/

#include "ecal_sample_struct_registration.h"

#include "cereal/archives/portable_binary.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "cereal/types/map.hpp"

namespace cereal
{
  // eCAL::Registration::OSInfo
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::OSInfo& os_info)
  {
    archive
    (
      os_info.osname                    // name
    );
  }

  // eCAL::Registration::Host
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::Host& host)
  {
    archive
    (
      host.hname,                       // host name
      host.os                           // operating system details
    );
  }

  // eCAL::Registration::ProcessState
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::ProcessState& process_state)
  {
    archive
    (
      process_state.severity,           // severity
      process_state.severity_level,     // severity level
      process_state.info                // info string
    );
  }

  // eCAL::Registration::LayerParUdpMC
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::LayerParUdpMC& layer_par_udpmc)
  {
    (void)archive;
    (void)layer_par_udpmc;
  }

  // eCAL::Registration::ConnectionPar
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::ConnectionPar& connection_par)
  {
    archive
    (
      connection_par.layer_par_udpmc    // parameter for ecal udp multicast
    );
  }

  // eCAL::Registration::TLayer
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::TLayer& tlayer)
  {
    archive
    (
      tlayer.type,                      // transport layer type
      tlayer.version,                   // transport layer version
      tlayer.confirmed,                 // transport layer used ?
      tlayer.par_layer                  // transport layer parameter
    );
  }

  // eCAL::Registration::Process
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::Process& process)
  {
    archive
    (
      process.rclock,                   // registration clock
      process.hname,                    // host name
      process.hgname,                   // host group name
      process.pid,                      // process id
      process.pname,                    // process name
      process.uname,                    // unit name
      process.pparam,                   // process parameter
      process.datawrite,                // data write bytes per sec
      process.dataread,                 // data read bytes per sec
      process.state,                    // process state info
      process.tsync_state,              // time synchronization state
      process.tsync_mod_name,           // time synchronization module name
      process.component_init_state,     // eCAL component initialization state (eCAL::Initialize(..))
      process.component_init_info,      // like comp_init_state as human readable string (pub|sub|srv|mon|log|time|proc)
      process.ecal_runtime_version      // loaded / runtime eCAL version of a component
    );
  }

  // eCAL::Registration::DataTypeInformation
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::DataTypeInformation& datatype_info)
  {
    archive
    (
      datatype_info.name,               // name of the datatype
      datatype_info.encoding,           // encoding of the datatype (e.g. protobuf, flatbuffers, capnproto)
      datatype_info.desc                // descriptor information of the datatype (necessary for reflection)
    );
  }

  // eCAL::Registration::Topic
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::Topic& topic)
  {
    archive
    (
      topic.rclock,                     // registration clock (heart beat)
      topic.hname,                      // host name
      topic.hgname,                     // host group name
      topic.pid,                        // process id
      topic.pname,                      // process name
      topic.uname,                      // unit name
      topic.tid,                        // topic id
      topic.tname,                      // topic name
      topic.direction,                  // direction (publisher, subscriber)
      topic.ttype,                      // topic type + topic encoding (deprecated)
      topic.tdesc,                      // topic description (protocol descriptor) (deprecated)
      topic.tdatatype,                  // topic datatype information (encoding & type & description)
      topic.tlayer,                     // active topic transport layers and it's specific parameter
      topic.tsize,                      // topic size
      topic.connections_loc,            // number of local connected entities
      topic.connections_ext,            // number of external connected entities
      topic.message_drops,              // dropped messages
      topic.did,                        // data send id (publisher setid)
      topic.dclock,                     // data clock (send / receive action)
      topic.dfreq,                      // data frequency (send / receive samples per second) [mHz]
      topic.attr                        // generic topic description
    );
  }

  // eCAL::Registration::Sample
  template <class Archive>
  void serialize(Archive& archive, eCAL::Registration::Sample& sample)
  {
    archive
    (
      sample.cmd_type,                  // sample command type
      sample.host,                      // host information
      sample.process,                   // process information
      sample.topic                      // topic information
    );                                  // topic content
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
