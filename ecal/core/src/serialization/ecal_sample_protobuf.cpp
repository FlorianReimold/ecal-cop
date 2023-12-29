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
 * @file   ecal_sample_protobuf.cpp
 * @brief  eCAL sample (de)serialization - protobuf variant
**/

#include "ecal_sample.h"

#ifdef _MSC_VER
  #pragma warning(push, 0) // disable proto warnings
#endif
#include <ecal/core/pb/ecal.pb.h>
#ifdef _MSC_VER
  #pragma warning(pop)
#endif

namespace
{
  eCAL::pb::Sample SampleStruct2Proto(const eCAL::Sample& sample_)
  {
    eCAL::pb::Sample pb_sample;

    // sample command type
    pb_sample.set_cmd_type(static_cast<eCAL::pb::eCmdType>(sample_.cmd_type));

    // host information
    eCAL::pb::Host* pb_host = pb_sample.mutable_host();
    pb_host->set_hname(sample_.host.hname);

    // process information
    eCAL::pb::Process* pb_process = pb_sample.mutable_process();
    pb_process->set_rclock(sample_.process.rclock);
    pb_process->set_hname(sample_.process.hname);
    pb_process->set_hgname(sample_.process.hgname);
    pb_process->set_pid(sample_.process.pid);
    pb_process->set_pname(sample_.process.pname);
    pb_process->set_uname(sample_.process.uname);
    pb_process->set_pparam(sample_.process.pparam);
    pb_process->set_pmemory(sample_.process.pmemory);
    pb_process->set_pcpu(sample_.process.pcpu);
    pb_process->set_usrptime(sample_.process.usrptime);
    pb_process->set_datawrite(sample_.process.datawrite);
    pb_process->set_dataread(sample_.process.dataread);
    pb_process->mutable_state()->set_severity(static_cast<eCAL::pb::eProcessSeverity>(sample_.process.state.severity));
    pb_process->mutable_state()->set_severity_level(static_cast<eCAL::pb::eProcessSeverityLevel>(sample_.process.state.severity_level));
    pb_process->mutable_state()->set_info(sample_.process.state.info);
    pb_process->set_tsync_state(static_cast<eCAL::pb::eTSyncState>(sample_.process.tsync_state));
    pb_process->set_tsync_mod_name(sample_.process.tsync_mod_name);
    pb_process->set_component_init_state(sample_.process.component_init_state);
    pb_process->set_component_init_info(sample_.process.component_init_info);
    pb_process->set_ecal_runtime_version(sample_.process.ecal_runtime_version);


    // topic information
    eCAL::pb::Topic* pb_topic = pb_sample.mutable_topic();
    pb_topic->set_rclock(sample_.topic.rclock);
    pb_topic->set_hname(sample_.topic.hname);
    pb_topic->set_hgname(sample_.topic.hgname);
    pb_topic->set_pid(sample_.topic.pid);
    pb_topic->set_pname(sample_.topic.pname);
    pb_topic->set_uname(sample_.topic.uname);
    pb_topic->set_tid(sample_.topic.tid);
    pb_topic->set_tname(sample_.topic.tname);
    pb_topic->set_direction(sample_.topic.direction);
    pb_topic->set_ttype(sample_.topic.ttype);
    pb_topic->set_tdesc(sample_.topic.tdesc.data(), sample_.topic.tdesc.size());
    pb_topic->mutable_tdatatype()->set_name(sample_.topic.tdatatype.name);
    pb_topic->mutable_tdatatype()->set_encoding(sample_.topic.tdatatype.encoding);
    pb_topic->mutable_tdatatype()->set_desc(sample_.topic.tdatatype.desc.data(), sample_.topic.tdatatype.desc.size());
    pb_topic->set_tsize(static_cast<int32_t>(sample_.topic.tsize));
    pb_topic->set_connections_loc(sample_.topic.connections_loc);
    pb_topic->set_connections_ext(sample_.topic.connections_ext);
    pb_topic->set_message_drops(static_cast<int32_t>(sample_.topic.message_drops));
    pb_topic->set_did(sample_.topic.did);
    pb_topic->set_dclock(sample_.topic.dclock);
    pb_topic->set_dfreq(sample_.topic.dfreq);

    for (const auto& layer : sample_.topic.tlayer)
    {
      eCAL::pb::TLayer* pb_layer = pb_topic->add_tlayer();
      pb_layer->set_type(static_cast<eCAL::pb::eTLayerType>(layer.type));
      pb_layer->set_version(layer.version);
      pb_layer->set_confirmed(layer.confirmed);
    }

    for (const auto& attr : sample_.topic.attr)
    {
      (*pb_topic->mutable_attr())[attr.first] = attr.second;
    }

    // topic content
    eCAL::pb::Content* pb_content = pb_sample.mutable_content();
    pb_content->set_id(sample_.content.id);
    pb_content->set_clock(sample_.content.clock);
    pb_content->set_time(sample_.content.time);
    pb_content->set_payload(sample_.content.payload.data(), sample_.content.payload.size());
    pb_content->set_hash(sample_.content.hash);

    return pb_sample;
  }

  eCAL::Sample Proto2SampleStruct(const eCAL::pb::Sample& pb_sample_)
  {
    eCAL::Sample sample;

    // sample command type
    sample.cmd_type = static_cast<eCAL::eCmdType>(pb_sample_.cmd_type());

    // host information
    const eCAL::pb::Host& pb_host = pb_sample_.host();
    sample.host.hname = pb_host.hname();

    // process information
    const eCAL::pb::Process& pb_process = pb_sample_.process();
    sample.process.rclock = pb_process.rclock();
    sample.process.hname = pb_process.hname();
    sample.process.hgname = pb_process.hgname();
    sample.process.pid = pb_process.pid();
    sample.process.pname = pb_process.pname();
    sample.process.uname = pb_process.uname();
    sample.process.pparam = pb_process.pparam();
    sample.process.pmemory = pb_process.pmemory();
    sample.process.pcpu = pb_process.pcpu();
    sample.process.usrptime = pb_process.usrptime();
    sample.process.datawrite = pb_process.datawrite();
    sample.process.dataread = pb_process.dataread();
    sample.process.state.severity = static_cast<eCAL::eProcessSeverity>(pb_process.state().severity());
    sample.process.state.severity_level = static_cast<eCAL::eProcessSeverityLevel>(pb_process.state().severity_level());
    sample.process.state.info = pb_process.state().info();
    sample.process.tsync_state = static_cast<eCAL::eTSyncState>(pb_process.tsync_state());
    sample.process.tsync_mod_name = pb_process.tsync_mod_name();
    sample.process.component_init_state = pb_process.component_init_state();
    sample.process.component_init_info = pb_process.component_init_info();
    sample.process.ecal_runtime_version = pb_process.ecal_runtime_version();

    // topic information
    const eCAL::pb::Topic& pb_topic = pb_sample_.topic();
    sample.topic.rclock = pb_topic.rclock();
    sample.topic.hname = pb_topic.hname();
    sample.topic.hgname = pb_topic.hgname();
    sample.topic.pid = pb_topic.pid();
    sample.topic.pname = pb_topic.pname();
    sample.topic.uname = pb_topic.uname();
    sample.topic.tid = pb_topic.tid();
    sample.topic.tname = pb_topic.tname();
    sample.topic.direction = pb_topic.direction();
    sample.topic.ttype = pb_topic.ttype();
    const std::string& tdesc_str = pb_topic.tdesc();
    sample.topic.tdesc.assign(tdesc_str.begin(), tdesc_str.end());
    sample.topic.tdatatype.name = pb_sample_.topic().tdatatype().name();
    sample.topic.tdatatype.encoding = pb_sample_.topic().tdatatype().encoding();
    const std::string& desc_str = pb_sample_.topic().tdatatype().desc();
    sample.topic.tdatatype.desc.assign(desc_str.begin(), desc_str.end());
    sample.topic.tsize = pb_topic.tsize();
    sample.topic.connections_loc = pb_topic.connections_loc();
    sample.topic.connections_ext = pb_topic.connections_ext();
    sample.topic.message_drops = pb_topic.message_drops();
    sample.topic.did = pb_topic.did();
    sample.topic.dclock = pb_topic.dclock();
    sample.topic.dfreq = pb_topic.dfreq();

    const auto& pb_tlayer_list = pb_topic.tlayer();
    sample.topic.tlayer.reserve(pb_tlayer_list.size());
    for (const auto& pb_tlayer : pb_tlayer_list)
    {
      eCAL::TLayer tlayer;
      tlayer.type = static_cast<eCAL::eTLayerType>(pb_tlayer.type());
      tlayer.version = pb_tlayer.version();
      tlayer.confirmed = pb_tlayer.confirmed();
      sample.topic.tlayer.push_back(tlayer);
    }

    const auto& pb_attr_map = pb_topic.attr();
    for (const auto& pb_attr : pb_attr_map)
    {
      sample.topic.attr.emplace(pb_attr.first, pb_attr.second);
    }

    // topic content
    const eCAL::pb::Content& pb_content = pb_sample_.content();
    sample.content.id = pb_content.id();
    sample.content.clock = pb_content.clock();
    sample.content.time = pb_content.time();
    const std::string& payload_str = pb_content.payload();
    sample.content.payload.assign(payload_str.begin(), payload_str.end());
    sample.content.hash = pb_content.hash();

    return sample;
  }
}

namespace eCAL
{
  std::string SerializeSampleAsString(const Sample& source_sample_)
  {
    eCAL::pb::Sample pb_sample = SampleStruct2Proto(source_sample_);
    return pb_sample.SerializeAsString();
  };

  bool ParseSampleFromArray(const void* data_, int size_, Sample& target_sample_)
  {
    eCAL::pb::Sample pb_sample;
    if (pb_sample.ParseFromArray(data_, size_))
    {
      target_sample_ = Proto2SampleStruct(pb_sample);
      return true;
    }
    return false;
  };
}
