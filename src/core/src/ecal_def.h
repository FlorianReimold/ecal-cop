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
 * @brief  eCAL global definition file
**/

#pragma once

#include <ecal/ecal_os.h>

/**********************************************************************************************/
/*                                     config settings                                        */
/**********************************************************************************************/
/* base data path name */
#define ECAL_HOME_PATH_WINDOWS                     ""
#define ECAL_HOME_PATH_LINUX                       ".ecal"
#define ECAL_LOG_PATH                              "logs"
#define ECAL_SETTINGS_PATH                         "cfg"

/* ini file name */
#define ECAL_DEFAULT_CFG                           "ecal.ini"

/**********************************************************************************************/
/*                                     logging settings                                       */
/**********************************************************************************************/
#define MON_LOG_FILTER_CON                         "info,warning,error,fatal"
#define MON_LOG_FILTER_FILE                        ""

/**********************************************************************************************/
/*                                     network settings                                       */
/**********************************************************************************************/
/* network switch */
#define NET_ENABLED                                false

/* eCAL udp multicast defines */
#define NET_UDP_MULTICAST_CONFIG_VERSION           "v1"
#define NET_UDP_MULTICAST_GROUP                    "239.0.0.1"
#define NET_UDP_MULTICAST_MASK                     "0.0.0.15"
#define NET_UDP_MULTICAST_PORT                     14000
#define NET_UDP_MULTICAST_TTL                      3
#define NET_UDP_MULTICAST_PORT_REG_OFF             0
#define NET_UDP_MULTICAST_PORT_SAMPLE_OFF          2
#define NET_UDP_MULTICAST_SNDBUF                   (5*1024*1024)  /* 5 MByte */
#define NET_UDP_MULTICAST_RCVBUF                   (5*1024*1024)  /* 5 MByte */
#define NET_UDP_MULTICAST_JOIN_ALL_IF_ENABLED      false

#define NET_UDP_RECBUFFER_TIMEOUT                  1000  /* ms */
#define NET_UDP_RECBUFFER_CLEANUP                  10    /* ms */

/* overall udp multicast bandwidth limitation in bytes/s, -1 == no limitation */
#define NET_BANDWIDTH_MAX_UDP                      (-1)

/* enable the receiving of udp payload */
#define NET_UDP_MC_REC_ENABLED                     true

/* use npcap on windows platform (enables much higher socket performance) */
#define NET_NPCAP_ENABLED                          false

/* common host group name that enables interprocess mechanisms across (virtual) host borders (e.g, Docker); by default equivalent to local host name */
#define NET_HOST_GROUP_NAME                        ""

/**********************************************************************************************/
/*                                     publisher settings                                     */
/**********************************************************************************************/
/* share topic type                  [          on = 1, off = 0] */
#define PUB_SHARE_TTYPE                            1
/* share topic description           [          on = 1, off = 0] */
#define PUB_SHARE_TDESC                            1

/**********************************************************************************************/
/*                                     time settings                                          */
/**********************************************************************************************/
#define TIME_SYNC_MOD_RT                           ""
#define TIME_SYNC_MOD_REPLAY                       ""

/**********************************************************************************************/
/*                                     ecal internal timings                                  */
/**********************************************************************************************/
/* timeout for automatic removing registered topics and memory files in global database in ms */
#define CMN_REGISTRATION_TO                        (60*1000)  /* ms */

/* time for resend registration info from publisher/subscriber in ms */
#define CMN_REGISTRATION_REFRESH                   1000  /* ms */

/* cylce time udp receive threads in ms */
#define CMN_UDP_RECEIVE_THREAD_CYCLE_TIME_MS       1000  /* ms */

/**********************************************************************************************/
/*                                     events                                                 */
/**********************************************************************************************/
/* common stop event prefix to shut down a local user process */
#define EVENT_SHUTDOWN_PROC                        "ecal_shutdown_process"

/**********************************************************************************************/
/*                                     experimental                                           */
/**********************************************************************************************/
/* enable dropping of payload messages that arrive out of order */
#define EXP_DROP_OUT_OF_ORDER_MESSAGES             false
