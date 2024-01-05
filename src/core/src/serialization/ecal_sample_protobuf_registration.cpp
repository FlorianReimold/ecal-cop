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
 * @file   ecal_sample_protobuf_registration.cpp
 * @brief  eCAL registration sample (de)serialization - protobuf variant
**/

#include "ecal_sample_struct_registration.h"

#ifdef _MSC_VER
  #pragma warning(push, 0) // disable proto warnings
#endif
#include <ecal/core/pb/ecal.pb.h>
#ifdef _MSC_VER
  #pragma warning(pop)
#endif

namespace
{
  eCAL::pb::Sample RegistrationStruct2Proto(const eCAL::Registration::Sample& source_sample_)
  {
    eCAL::pb::Sample pb_registration;

    // registration command type
    pb_registration.set_cmd_type(static_cast<eCAL::pb::eCmdType>(source_sample_.cmd_type));

    // host information
    eCAL::pb::Host* pb_host = pb_registration.mutable_host();
    pb_host->set_hname(source_sample_.host.hname);

    // process information
    eCAL::pb::Process* pb_process = pb_registration.mutable_process();
    pb_process->set_rclock(source_sample_.process.rclock);
    pb_process->set_hname(source_sample_.process.hname);
    pb_process->set_hgname(source_sample_.process.hgname);
    pb_process->set_pid(source_sample_.process.pid);
    pb_process->set_pname(source_sample_.process.pname);
    pb_process->set_uname(source_sample_.process.uname);
    pb_process->set_pparam(source_sample_.process.pparam);
    pb_process->set_datawrite(source_sample_.process.datawrite);
    pb_process->set_dataread(source_sample_.process.dataread);
    pb_process->mutable_state()->set_severity(static_cast<eCAL::pb::eProcessSeverity>(source_sample_.process.state.severity));
    pb_process->mutable_state()->set_severity_level(static_cast<eCAL::pb::eProcessSeverityLevel>(source_sample_.process.state.severity_level));
    pb_process->mutable_state()->set_info(source_sample_.process.state.info);
    pb_process->set_tsync_state(static_cast<eCAL::pb::eTSyncState>(source_sample_.process.tsync_state));
    pb_process->set_tsync_mod_name(source_sample_.process.tsync_mod_name);
    pb_process->set_component_init_state(source_sample_.process.component_init_state);
    pb_process->set_component_init_info(source_sample_.process.component_init_info);
    pb_process->set_ecal_runtime_version(source_sample_.process.ecal_runtime_version);


    // topic information
    eCAL::pb::Topic* pb_topic = pb_registration.mutable_topic();
    pb_topic->set_rclock(source_sample_.topic.rclock);
    pb_topic->set_hname(source_sample_.topic.hname);
    pb_topic->set_hgname(source_sample_.topic.hgname);
    pb_topic->set_pid(source_sample_.topic.pid);
    pb_topic->set_pname(source_sample_.topic.pname);
    pb_topic->set_uname(source_sample_.topic.uname);
    pb_topic->set_tid(source_sample_.topic.tid);
    pb_topic->set_tname(source_sample_.topic.tname);
    pb_topic->set_direction(source_sample_.topic.direction);
    pb_topic->set_ttype(source_sample_.topic.ttype);
    pb_topic->set_tdesc(source_sample_.topic.tdesc.data(), source_sample_.topic.tdesc.size());
    pb_topic->mutable_tdatatype()->set_name(source_sample_.topic.tdatatype.name);
    pb_topic->mutable_tdatatype()->set_encoding(source_sample_.topic.tdatatype.encoding);
    pb_topic->mutable_tdatatype()->set_desc(source_sample_.topic.tdatatype.desc.data(), source_sample_.topic.tdatatype.desc.size());
    pb_topic->set_tsize(source_sample_.topic.tsize);
    pb_topic->set_connections_loc(source_sample_.topic.connections_loc);
    pb_topic->set_connections_ext(source_sample_.topic.connections_ext);
    pb_topic->set_message_drops(source_sample_.topic.message_drops);
    pb_topic->set_did(source_sample_.topic.did);
    pb_topic->set_dclock(source_sample_.topic.dclock);
    pb_topic->set_dfreq(source_sample_.topic.dfreq);

    for (const auto& layer : source_sample_.topic.tlayer)
    {
      eCAL::pb::TLayer* pb_layer = pb_topic->add_tlayer();
      pb_layer->set_type(static_cast<eCAL::pb::eTLayerType>(layer.type));
      pb_layer->set_version(layer.version);
      pb_layer->set_confirmed(layer.confirmed);
    }

    for (const auto& attr : source_sample_.topic.attr)
    {
      (*pb_topic->mutable_attr())[attr.first] = attr.second;
    }

    return pb_registration;
  }

  eCAL::Registration::Sample Proto2RegistrationStruct(const eCAL::pb::Sample& pb_source_sample_)
  {
    eCAL::Registration::Sample registration;

    // registration command type
    registration.cmd_type = static_cast<eCAL::eCmdType>(pb_source_sample_.cmd_type());

    // host information
    const eCAL::pb::Host& pb_host = pb_source_sample_.host();
    registration.host.hname = pb_host.hname();

    // process information
    const eCAL::pb::Process& pb_process = pb_source_sample_.process();
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
    const eCAL::pb::Topic& pb_topic = pb_source_sample_.topic();
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
    registration.topic.tdatatype.name = pb_source_sample_.topic().tdatatype().name();
    registration.topic.tdatatype.encoding = pb_source_sample_.topic().tdatatype().encoding();
    const std::string& desc_str = pb_source_sample_.topic().tdatatype().desc();
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

    return registration;
  }
}

namespace eCAL
{
  bool SerializeToBuffer(const Registration::Sample& source_sample_, std::vector<char>& target_buffer_)
  {
    eCAL::pb::Sample pb_sample = RegistrationStruct2Proto(source_sample_);
    target_buffer_.resize(pb_sample.ByteSizeLong());
    return pb_sample.SerializeToArray(target_buffer_.data(), static_cast<int>(target_buffer_.size()));
  }

  bool DeserializeFromBuffer(const char* data_, size_t size_, Registration::Sample& target_sample_)
  {
    eCAL::pb::Sample pb_sample;
    if (pb_sample.ParseFromArray(data_, static_cast<int>(size_)))
    {
      target_sample_ = Proto2RegistrationStruct(pb_sample);
      return true;
    }
    return false;
  }
}
