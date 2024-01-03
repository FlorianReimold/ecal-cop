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
 * @file   ecal_sample_nanopb_registration.cpp
 * @brief  eCAL registration sample (de)serialization - nanopb variant
**/

#include "ecal_sample_struct_registration.h"

#include "nanopb/ecal.pb.h"
#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"

namespace
{
  eCAL_pb_Sample RegistrationStruct2Proto(const eCAL::Registration::Sample& registration_)
  {
    eCAL_pb_Sample pb_registration;

#if 0

    // registration command type
    pb_registration.set_cmd_type(static_cast<eCAL::pb::eCmdType>(registration_.cmd_type));

    // host information
    eCAL::pb::Host* pb_host = pb_registration.mutable_host();
    pb_host->set_hname(registration_.host.hname);

    // process information
    eCAL::pb::Process* pb_process = pb_registration.mutable_process();
    pb_process->set_rclock(registration_.process.rclock);
    pb_process->set_hname(registration_.process.hname);
    pb_process->set_hgname(registration_.process.hgname);
    pb_process->set_pid(registration_.process.pid);
    pb_process->set_pname(registration_.process.pname);
    pb_process->set_uname(registration_.process.uname);
    pb_process->set_pparam(registration_.process.pparam);
    pb_process->set_datawrite(registration_.process.datawrite);
    pb_process->set_dataread(registration_.process.dataread);
    pb_process->mutable_state()->set_severity(static_cast<eCAL::pb::eProcessSeverity>(registration_.process.state.severity));
    pb_process->mutable_state()->set_severity_level(static_cast<eCAL::pb::eProcessSeverityLevel>(registration_.process.state.severity_level));
    pb_process->mutable_state()->set_info(registration_.process.state.info);
    pb_process->set_tsync_state(static_cast<eCAL::pb::eTSyncState>(registration_.process.tsync_state));
    pb_process->set_tsync_mod_name(registration_.process.tsync_mod_name);
    pb_process->set_component_init_state(registration_.process.component_init_state);
    pb_process->set_component_init_info(registration_.process.component_init_info);
    pb_process->set_ecal_runtime_version(registration_.process.ecal_runtime_version);


    // topic information
    eCAL::pb::Topic* pb_topic = pb_registration.mutable_topic();
    pb_topic->set_rclock(registration_.topic.rclock);
    pb_topic->set_hname(registration_.topic.hname);
    pb_topic->set_hgname(registration_.topic.hgname);
    pb_topic->set_pid(registration_.topic.pid);
    pb_topic->set_pname(registration_.topic.pname);
    pb_topic->set_uname(registration_.topic.uname);
    pb_topic->set_tid(registration_.topic.tid);
    pb_topic->set_tname(registration_.topic.tname);
    pb_topic->set_direction(registration_.topic.direction);
    pb_topic->set_ttype(registration_.topic.ttype);
    pb_topic->set_tdesc(registration_.topic.tdesc.data(), registration_.topic.tdesc.size());
    pb_topic->mutable_tdatatype()->set_name(registration_.topic.tdatatype.name);
    pb_topic->mutable_tdatatype()->set_encoding(registration_.topic.tdatatype.encoding);
    pb_topic->mutable_tdatatype()->set_desc(registration_.topic.tdatatype.desc.data(), registration_.topic.tdatatype.desc.size());
    pb_topic->set_tsize(registration_.topic.tsize);
    pb_topic->set_connections_loc(registration_.topic.connections_loc);
    pb_topic->set_connections_ext(registration_.topic.connections_ext);
    pb_topic->set_message_drops(registration_.topic.message_drops);
    pb_topic->set_did(registration_.topic.did);
    pb_topic->set_dclock(registration_.topic.dclock);
    pb_topic->set_dfreq(registration_.topic.dfreq);

    for (const auto& layer : registration_.topic.tlayer)
    {
      eCAL::pb::TLayer* pb_layer = pb_topic->add_tlayer();
      pb_layer->set_type(static_cast<eCAL::pb::eTLayerType>(layer.type));
      pb_layer->set_version(layer.version);
      pb_layer->set_confirmed(layer.confirmed);
    }

    for (const auto& attr : registration_.topic.attr)
    {
      (*pb_topic->mutable_attr())[attr.first] = attr.second;
    }

#endif

    return pb_registration;
  }

  eCAL::Registration::Sample Proto2RegistrationStruct(const eCAL_pb_Sample& pb_registration_)
  {
    eCAL::Registration::Sample registration;

#if 0

    // registration command type
    registration.cmd_type = static_cast<eCAL::eCmdType>(pb_registration_.cmd_type());

    // host information
    const eCAL::pb::Host& pb_host = pb_registration_.host();
    registration.host.hname = pb_host.hname();

    // process information
    const eCAL::pb::Process& pb_process = pb_registration_.process();
    registration.process.rclock = pb_process.rclock();
    registration.process.hname = pb_process.hname();
    registration.process.hgname = pb_process.hgname();
    registration.process.pid = pb_process.pid();
    registration.process.pname = pb_process.pname();
    registration.process.uname = pb_process.uname();
    registration.process.pparam = pb_process.pparam();
    registration.process.datawrite = pb_process.datawrite();
    registration.process.dataread = pb_process.dataread();
    registration.process.state.severity = static_cast<eCAL::Registration::eProcessSeverity>(pb_process.state().severity());
    registration.process.state.severity_level = static_cast<eCAL::Registration::eProcessSeverityLevel>(pb_process.state().severity_level());
    registration.process.state.info = pb_process.state().info();
    registration.process.tsync_state = static_cast<eCAL::Registration::eTSyncState>(pb_process.tsync_state());
    registration.process.tsync_mod_name = pb_process.tsync_mod_name();
    registration.process.component_init_state = pb_process.component_init_state();
    registration.process.component_init_info = pb_process.component_init_info();
    registration.process.ecal_runtime_version = pb_process.ecal_runtime_version();

    // topic information
    const eCAL::pb::Topic& pb_topic = pb_registration_.topic();
    registration.topic.rclock = pb_topic.rclock();
    registration.topic.hname = pb_topic.hname();
    registration.topic.hgname = pb_topic.hgname();
    registration.topic.pid = pb_topic.pid();
    registration.topic.pname = pb_topic.pname();
    registration.topic.uname = pb_topic.uname();
    registration.topic.tid = pb_topic.tid();
    registration.topic.tname = pb_topic.tname();
    registration.topic.direction = pb_topic.direction();
    registration.topic.ttype = pb_topic.ttype();
    const std::string& tdesc_str = pb_topic.tdesc();
    registration.topic.tdesc.assign(tdesc_str.begin(), tdesc_str.end());
    registration.topic.tdatatype.name = pb_registration_.topic().tdatatype().name();
    registration.topic.tdatatype.encoding = pb_registration_.topic().tdatatype().encoding();
    const std::string& desc_str = pb_registration_.topic().tdatatype().desc();
    registration.topic.tdatatype.desc.assign(desc_str.begin(), desc_str.end());
    registration.topic.tsize = pb_topic.tsize();
    registration.topic.connections_loc = pb_topic.connections_loc();
    registration.topic.connections_ext = pb_topic.connections_ext();
    registration.topic.message_drops = pb_topic.message_drops();
    registration.topic.did = pb_topic.did();
    registration.topic.dclock = pb_topic.dclock();
    registration.topic.dfreq = pb_topic.dfreq();

    const auto& pb_tlayer_list = pb_topic.tlayer();
    registration.topic.tlayer.reserve(pb_tlayer_list.size());
    for (const auto& pb_tlayer : pb_tlayer_list)
    {
      eCAL::Registration::TLayer tlayer{};
      tlayer.type = static_cast<eCAL::eTLayerType>(pb_tlayer.type());
      tlayer.version = pb_tlayer.version();
      tlayer.confirmed = pb_tlayer.confirmed();
      registration.topic.tlayer.push_back(tlayer);
    }

    const auto& pb_attr_map = pb_topic.attr();
    for (const auto& pb_attr : pb_attr_map)
    {
      registration.topic.attr.emplace(pb_attr.first, pb_attr.second);
    }

#endif

    return registration;
  }
}

namespace eCAL
{
  std::string SerializeToBinaryString(const Registration::Sample& registration_sample_)
  {
    return "";
  }

  bool DeserializeFromBuffer(const char* data_, size_t size_, Registration::Sample& registration_sample_)
  {
    return false;
  }
}
