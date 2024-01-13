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
#include "ecal_sample_nanopb_common.h"

#include "nanopb/ecal.pb.h"
#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"

#include <iostream>

namespace
{
  bool RegistrationStruct2Buffer(const eCAL::Registration::Sample& source_sample_, std::vector<char>& target_buffer_)
  {
    eCAL_pb_Sample pb_sample = eCAL_pb_Sample_init_default;

    // command type
    pb_sample.cmd_type = static_cast<eCAL_pb_eCmdType>(source_sample_.cmd_type);

    ///////////////////////////////////////////////
    // host information
    ///////////////////////////////////////////////
    pb_sample.has_host = true;

    // hname
    eCAL::nanopb::encode_string(pb_sample.host.hname, source_sample_.host.hname);

    ///////////////////////////////////////////////
    // process information
    ///////////////////////////////////////////////
    pb_sample.has_process = true;

    // rclock
    pb_sample.process.rclock = source_sample_.process.rclock;
    // hname
    eCAL::nanopb::encode_string(pb_sample.process.hname, source_sample_.process.hname);
    // hgname
    eCAL::nanopb::encode_string(pb_sample.process.hgname, source_sample_.process.hgname);
    // pid
    pb_sample.process.pid = source_sample_.process.pid;
    // pname
    eCAL::nanopb::encode_string(pb_sample.process.pname, source_sample_.process.pname);
    // uname
    eCAL::nanopb::encode_string(pb_sample.process.uname, source_sample_.process.uname);
    // pparam
    eCAL::nanopb::encode_string(pb_sample.process.pparam, source_sample_.process.pparam);
    // datawrite
    pb_sample.process.datawrite = source_sample_.process.datawrite;
    // dataread
    pb_sample.process.dataread = source_sample_.process.dataread;
    // state.severity
    pb_sample.process.state.severity = static_cast<eCAL_pb_eProcessSeverity>(source_sample_.process.state.severity);
    // state.severity_level
    pb_sample.process.state.severity_level = static_cast<eCAL_pb_eProcessSeverityLevel>(source_sample_.process.state.severity_level);
    // state.info
    eCAL::nanopb::encode_string(pb_sample.process.state.info, source_sample_.process.state.info);
    // process.tsync_state
    pb_sample.process.tsync_state = static_cast<eCAL_pb_eTSyncState>(source_sample_.process.tsync_state);
    // tsync_mod_name
    eCAL::nanopb::encode_string(pb_sample.process.tsync_mod_name, source_sample_.process.tsync_mod_name);
    // component_init_state
    pb_sample.process.component_init_state = source_sample_.process.component_init_state;
    // component_init_info
    eCAL::nanopb::encode_string(pb_sample.process.component_init_info, source_sample_.process.component_init_info);
    // ecal_runtime_version
    eCAL::nanopb::encode_string(pb_sample.process.ecal_runtime_version, source_sample_.process.ecal_runtime_version);

    ///////////////////////////////////////////////
    // topic information
    ///////////////////////////////////////////////
    pb_sample.has_topic = true;

    // rclock
    pb_sample.topic.rclock = source_sample_.topic.rclock;
    // hname
    eCAL::nanopb::encode_string(pb_sample.topic.hname, source_sample_.topic.hname);
    // hgname
    eCAL::nanopb::encode_string(pb_sample.topic.hgname, source_sample_.topic.hgname);
    // pid
    pb_sample.topic.pid = source_sample_.topic.pid;
    // pname (not needed for topic registration)
    //eCAL::nanopb::encode_string(pb_sample.topic.pname, source_sample_.topic.pname);
    // uname
    eCAL::nanopb::encode_string(pb_sample.topic.uname, source_sample_.topic.uname);
    // tid
    eCAL::nanopb::encode_string(pb_sample.topic.tid, source_sample_.topic.tid);
    // tname
    eCAL::nanopb::encode_string(pb_sample.topic.tname, source_sample_.topic.tname);
    // direction
    eCAL::nanopb::encode_string(pb_sample.topic.direction, source_sample_.topic.direction);
    // ttype
    eCAL::nanopb::encode_string(pb_sample.topic.ttype, source_sample_.topic.ttype);
    // tdesc
    eCAL::nanopb::encode_string(pb_sample.topic.tdesc, source_sample_.topic.tdesc);
    // tdatatype.name
    eCAL::nanopb::encode_string(pb_sample.topic.tdatatype.name, source_sample_.topic.tdatatype.name);
    // tdatatype.encoding
    eCAL::nanopb::encode_string(pb_sample.topic.tdatatype.encoding, source_sample_.topic.tdatatype.encoding);
    // tdatatype.desc
    eCAL::nanopb::encode_string(pb_sample.topic.tdatatype.desc, source_sample_.topic.tdatatype.desc);
    // tsize
    pb_sample.topic.tsize = source_sample_.topic.tsize;
    // connections_loc
    pb_sample.topic.connections_loc = source_sample_.topic.connections_loc;
    // connections_ext
    pb_sample.topic.connections_ext = source_sample_.topic.connections_ext;
    // message_drops
    pb_sample.topic.message_drops = source_sample_.topic.message_drops;
    // did
    pb_sample.topic.did = source_sample_.topic.did;
    // dclock
    pb_sample.topic.dclock = source_sample_.topic.dclock;
    // dfreq
    pb_sample.topic.dfreq = source_sample_.topic.dfreq;
    // tlayer
    eCAL::nanopb::encode_registration_layer(pb_sample.topic.tlayer, source_sample_.topic.tlayer);
    // attr
    eCAL::nanopb::encode_map(pb_sample.topic.attr, source_sample_.topic.attr);

    ///////////////////////////////////////////////
    // evaluate byte size
    ///////////////////////////////////////////////
    pb_ostream_t pb_sizestream = { nullptr };
    pb_encode(&pb_sizestream, eCAL_pb_Sample_fields, &pb_sample);

    ///////////////////////////////////////////////
    // encode it
    ///////////////////////////////////////////////
    target_buffer_.resize(pb_sizestream.bytes_written);
    pb_ostream_t pb_ostream;
    pb_ostream = pb_ostream_from_buffer((pb_byte_t*)(target_buffer_.data()), target_buffer_.size());
    if (!pb_encode(&pb_ostream, eCAL_pb_Sample_fields, &pb_sample))
    {
      std::cerr << "NanoPb registration encode failed: " << pb_ostream.errmsg << std::endl;
    }
    else
    {
      return true;
    }

    return false;
  }

  bool Buffer2RegistrationStruct(const char* data_, size_t size_, eCAL::Registration::Sample& registration_)
  {
    if (data_ == nullptr) return false;
    if (size_ == 0)       return false;

    // initialize
    eCAL_pb_Sample pb_sample = eCAL_pb_Sample_init_default;

    ///////////////////////////////////////////////
    // assign decoder
    ///////////////////////////////////////////////

    ///////////////////////////////////////////////
    // host information
    ///////////////////////////////////////////////
    // hname
    eCAL::nanopb::decode_string(pb_sample.host.hname, registration_.host.hname);

    ///////////////////////////////////////////////
    // process information
    ///////////////////////////////////////////////
    // hname
    eCAL::nanopb::decode_string(pb_sample.process.hname, registration_.process.hname);
    // hgname
    eCAL::nanopb::decode_string(pb_sample.process.hgname, registration_.process.hgname);    
    // pname
    eCAL::nanopb::decode_string(pb_sample.process.pname, registration_.process.pname);
    // uname
    eCAL::nanopb::decode_string(pb_sample.process.uname, registration_.process.uname);
    // pparam
    eCAL::nanopb::decode_string(pb_sample.process.pparam, registration_.process.pparam);
    // state.info
    eCAL::nanopb::decode_string(pb_sample.process.state.info, registration_.process.state.info);
    // tsync_mod_name
    eCAL::nanopb::decode_string(pb_sample.process.tsync_mod_name, registration_.process.tsync_mod_name);
    // component_init_info
    eCAL::nanopb::decode_string(pb_sample.process.component_init_info, registration_.process.component_init_info);
    // ecal_runtime_version
    eCAL::nanopb::decode_string(pb_sample.process.ecal_runtime_version, registration_.process.ecal_runtime_version);

    ///////////////////////////////////////////////
    // topic information
    ///////////////////////////////////////////////
    // hname
    eCAL::nanopb::decode_string(pb_sample.topic.hname, registration_.topic.hname);
    // hgname
    eCAL::nanopb::decode_string(pb_sample.topic.hgname, registration_.topic.hgname);
    // pname (not needed for topic registration)
    //eCAL::nanopb::decode_string(pb_sample.topic.pname, registration_.topic.pname);
    // uname
    eCAL::nanopb::decode_string(pb_sample.topic.uname, registration_.topic.uname);
    // tid
    eCAL::nanopb::decode_string(pb_sample.topic.tid, registration_.topic.tid);
    // tname
    eCAL::nanopb::decode_string(pb_sample.topic.tname, registration_.topic.tname);
    // direction
    eCAL::nanopb::decode_string(pb_sample.topic.direction, registration_.topic.direction);
    // ttype
    eCAL::nanopb::decode_string(pb_sample.topic.ttype, registration_.topic.ttype);
    // tdesc
    eCAL::nanopb::decode_string(pb_sample.topic.tdesc, registration_.topic.tdesc);
    // tdatatype.name
    eCAL::nanopb::decode_string(pb_sample.topic.tdatatype.name, registration_.topic.tdatatype.name);
    // tdatatype.encoding
    eCAL::nanopb::decode_string(pb_sample.topic.tdatatype.encoding, registration_.topic.tdatatype.encoding);
    // tdatatype.desc
    eCAL::nanopb::decode_string(pb_sample.topic.tdatatype.desc, registration_.topic.tdatatype.desc);
    // tlayer
    eCAL::nanopb::decode_registration_layer(pb_sample.topic.tlayer, registration_.topic.tlayer);
    // attr
    eCAL::nanopb::decode_map(pb_sample.topic.attr, registration_.topic.attr);

    ///////////////////////////////////////////////
    // decode it
    ///////////////////////////////////////////////
    pb_istream_t pb_istream;
    pb_istream = pb_istream_from_buffer((pb_byte_t*)data_, size_);
    if (!pb_decode(&pb_istream, eCAL_pb_Sample_fields, &pb_sample))
    {
      std::cerr << "NanoPb registration decode failed: " << pb_istream.errmsg << std::endl;
    }

    ///////////////////////////////////////////////
    // assign values
    ///////////////////////////////////////////////
    // command type
    registration_.cmd_type = static_cast<eCAL::eCmdType>(pb_sample.cmd_type);

    ///////////////////////////////////////////////
    // process information
    ///////////////////////////////////////////////
    // rclock
    registration_.process.rclock = pb_sample.process.rclock;
    // pid
    registration_.process.pid = pb_sample.process.pid;
    // datawrite
    registration_.process.datawrite = pb_sample.process.datawrite;
    // dataread
    registration_.process.dataread = pb_sample.process.dataread;
    // state.severity
    registration_.process.state.severity = static_cast<eCAL::Registration::eProcessSeverity>(pb_sample.process.state.severity);
    // state.severity_level
    registration_.process.state.severity_level = static_cast<eCAL::Registration::eProcessSeverityLevel>(pb_sample.process.state.severity_level);
    // tsync_state
    registration_.process.tsync_state = static_cast<eCAL::Registration::eTSyncState>(pb_sample.process.tsync_state);
    // component_init_state
    registration_.process.component_init_state = pb_sample.process.component_init_state;

    ///////////////////////////////////////////////
    // topic information
    ///////////////////////////////////////////////
    // rclock
    registration_.topic.rclock = pb_sample.topic.rclock;
    // pid
    registration_.topic.pid = pb_sample.topic.pid;
    // tsize
    registration_.topic.tsize = pb_sample.topic.tsize;
    // connections_loc
    registration_.topic.connections_loc = pb_sample.topic.connections_loc;
    // connections_ext
    registration_.topic.connections_ext = pb_sample.topic.connections_ext;
    // message_drops
    registration_.topic.message_drops = pb_sample.topic.message_drops;
    // did
    registration_.topic.did = pb_sample.topic.did;
    // dclock
    registration_.topic.dclock = pb_sample.topic.dclock;
    // dfreq
    registration_.topic.dfreq = pb_sample.topic.dfreq;

    return true;
  }
}

namespace eCAL
{
  bool SerializeToBuffer(const Registration::Sample& source_sample_, std::vector<char>& target_buffer_)
  {
    target_buffer_.clear();
    return RegistrationStruct2Buffer(source_sample_, target_buffer_);
  }

  bool DeserializeFromBuffer(const char* data_, size_t size_, Registration::Sample& registration_sample_)
  {
    return Buffer2RegistrationStruct(data_, size_, registration_sample_);
  }
}
