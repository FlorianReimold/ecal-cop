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
 * @file   ecal_sample_struct.h
 * @brief  eCAL sample as struct
**/

#pragma once

#include <map>
#include <string>
#include <vector>

namespace eCAL
{
  enum eProcessSeverity
  {
    proc_sev_unknown  = 0,
    proc_sev_healthy  = 1,
    proc_sev_warning  = 2,
    proc_sev_critical = 3,
    proc_sev_failed   = 4
  };

  enum eProcessSeverityLevel
  {
    proc_sev_level_unknown = 0,
    proc_sev_level1        = 1,
    proc_sev_level2        = 2,
    proc_sev_level3        = 3,
    proc_sev_level4        = 4,
    proc_sev_level5        = 5
  };

  enum eTSyncState
  {
    tsync_none     = 0,
    tsync_realtime = 1,
    tsync_replay   = 2
  };

  enum eCmdType
  {
    bct_none             = 0,
    bct_set_sample       = 1,
    bct_reg_publisher    = 2,
    bct_reg_subscriber   = 3,
    bct_reg_process      = 4,
    bct_unreg_publisher  = 12,
    bct_unreg_subscriber = 13,
    bct_unreg_process    = 14
  };

  enum eTLayerType
  {
    tl_none        = 0,
    tl_ecal_udp_mc = 1,
    tl_all         = 255
  };

  // Operating system details
  struct OSInfo
  {
    std::string                         osname;                 // name
  };

  // eCAL host
  struct Host
  {
    std::string                         hname;                  // host name
    OSInfo                              os;                     // operating system details
  };

  // Process severity information
  struct ProcessState
  {
    eProcessSeverity                    severity;               // severity
    eProcessSeverityLevel               severity_level;         // severity level
    std::string                         info;                   // info string
  };

  // Transport layer parameters for ecal udp multicast
  struct LayerParUdpMC
  {
  };

  // Connection parameter for reader/writer
  struct ConnectionPar
  {
    LayerParUdpMC                       layer_par_udpmc;        // parameter for ecal udp multicast
  };

  // Transport layer information
  struct TLayer
  {
    eTLayerType                         type;                   // transport layer type
    int32_t                             version;                // transport layer version
    bool                                confirmed;              // transport layer used?
    ConnectionPar                       par_layer;              // transport layer parameter
  };

  // Process information
  struct SProcess
  {
    int32_t                             rclock;                 // registration clock
    std::string                         hname;                  // host name
    std::string                         hgname;                 // host group name
    int32_t                             pid;                    // process id
    std::string                         pname;                  // process name
    std::string                         uname;                  // unit name
    std::string                         pparam;                 // process parameter
    int64_t                             pmemory;                // process memory
    float                               pcpu;                   // process cpu usage
    float                               usrptime;               // process user time
    int64_t                             datawrite;              // data write bytes per sec
    int64_t                             dataread;               // data read bytes per sec
    ProcessState                        state;                  // process state info
    eTSyncState                         tsync_state;            // time synchronization state
    std::string                         tsync_mod_name;         // time synchronization module name
    int32_t                             component_init_state;   // eCAL component initialization state (eCAL::Initialize(..))
    std::string                         component_init_info;    // like comp_init_state as a human-readable string (pub|sub|srv|mon|log|time|proc)
    std::string                         ecal_runtime_version;   // loaded/runtime eCAL version of a component
  };

  // Data type information
  struct DataTypeInformation
  {
    std::string                         name;                   // name of the datatype
    std::string                         encoding;               // encoding of the datatype (e.g., protobuf, flatbuffers, capnproto)
    std::string                         desc;                   // descriptor information of the datatype (necessary for reflection)
  };

  // eCAL topic information
  struct Topic
  {
    int32_t                             rclock;                 // registration clock (heart beat)
    std::string                         hname;                  // host name
    std::string                         hgname;                 // host group name
    int32_t                             pid;                    // process id
    std::string                         pname;                  // process name
    std::string                         uname;                  // unit name
    std::string                         tid;                    // topic id
    std::string                         tname;                  // topic name
    std::string                         direction;              // direction (publisher, subscriber)
    std::string                         ttype;                  // topic type + topic encoding (deprecated)
    std::string                         tdesc;                  // topic description (protocol descriptor) (deprecated)

    DataTypeInformation                 tdatatype;              // topic datatype information (encoding & type & description)

    std::vector<TLayer>                 tlayer;                 // active topic transport layers and its specific parameter
    int64_t                             tsize;                  // topic size

    int32_t                             connections_loc;        // number of local connected entities
    int32_t                             connections_ext;        // number of external connected entities
    int64_t                             message_drops;          // dropped messages

    int64_t                             did;                    // data send id (publisher setid)
    int64_t                             dclock;                 // data clock (send / receive action)
    int32_t                             dfreq;                  // data frequency (send / receive samples per second) [mHz]

    std::map<std::string, std::string>  attr;                   // generic topic description
  };

  // Topic content
  struct Content
  {
    int64_t                             id;                     // sample id
    int64_t                             clock;                  // internal used clock
    int64_t                             time;                   // time the content was updated
    std::vector<char>                   payload;                // octet stream
    int64_t                             hash;                   // unique hash for that sample
  };

  // Command type
  struct Sample
  {
    eCmdType                            cmd_type;               // sample command type
    Host                                host;                   // host information
    SProcess                            process;                // process information
    Topic                               topic;                  // topic information
    Content                             content;                // topic content
  };
}
