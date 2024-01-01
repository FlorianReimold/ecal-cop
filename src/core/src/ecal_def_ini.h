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
 * @brief  eCAL ini file keys
**/

#pragma once

/////////////////////////////////////
// common
/////////////////////////////////////
#define  CMN_SECTION_S                             "common"
#define  CMN_REGISTRATION_TO_S                     "registration_timeout"
#define  CMN_REGISTRATION_REFRESH_S                "registration_refresh"

/////////////////////////////////////
// network
/////////////////////////////////////
#define  NET_SECTION_S                             "network"

#define  NET_ENABLED_S                             "network_enabled"

#define  NET_UDP_MULTICAST_CONFIG_VERSION_S        "multicast_config_version"
#define  NET_UDP_MULTICAST_GROUP_S                 "multicast_group"
#define  NET_UDP_MULTICAST_MASK_S                  "multicast_mask"
#define  NET_UDP_MULTICAST_PORT_S                  "multicast_port"
#define  NET_UDP_MULTICAST_TTL_S                   "multicast_ttl"

#define  NET_UDP_MULTICAST_SNDBUF_S                "multicast_sndbuf"
#define  NET_UDP_MULTICAST_RCVBUF_S                "multicast_rcvbuf"

#define  NET_UDP_MULTICAST_JOIN_ALL_IF_ENABLED_S   "multicast_join_all_if"

#define  NET_BANDWIDTH_MAX_UDP_S                   "bandwidth_max_udp"

#define  NET_UDP_MC_REC_ENABLED_S                  "udp_mc_rec_enabled"

#define  NET_NPCAP_ENABLED_S                       "npcap_enabled"

#define  NET_HOST_GROUP_NAME_S                     "host_group_name"

/////////////////////////////////////
// time
/////////////////////////////////////
#define  TIME_SECTION_S                            "time"
#define  TIME_SYNC_MOD_RT_S                        "timesync_module_rt"
#define  TIME_SYNC_MOD_REPLAY_S                    "timesync_module_replay"

/////////////////////////////////////
// logging
/////////////////////////////////////
#define  MON_SECTION_S                             "monitoring"

#define  MON_LOG_FILTER_CON_S                      "filter_log_con"
#define  MON_LOG_FILTER_FILE_S                     "filter_log_file"

/////////////////////////////////////
// publisher
/////////////////////////////////////
#define  PUB_SECTION_S                             "publisher"

#define  PUB_SHARE_TTYPE_S                         "share_ttype"
#define  PUB_SHARE_TDESC_S                         "share_tdesc"

/////////////////////////////////////
// experimental
/////////////////////////////////////
#define  EXP_SECTION_S                             "experimental"

#define  EXP_DROP_OUT_OF_ORDER_MESSAGES_S          "drop_out_of_order_messages"
