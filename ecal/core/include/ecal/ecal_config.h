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

#pragma once

#include <ecal/ecal_os.h>
#include <ecal/ecal_log_level.h>

#include <string>

//@{ 
namespace eCAL
{
  namespace Config
  {
    enum class UdpConfigVersion
    {
      V1 = 1, // Legacy
      V2 = 2
    };

    /////////////////////////////////////
    // common
    /////////////////////////////////////

    ECAL_API std::string       GetLoadedEcalIniPath                 ();
    ECAL_API int               GetRegistrationTimeoutMs             ();
    ECAL_API int               GetRegistrationRefreshMs             ();

    /////////////////////////////////////
    // network
    /////////////////////////////////////

    ECAL_API bool              IsNetworkEnabled                     ();
    ECAL_API UdpConfigVersion  GetUdpMulticastConfigVersion         ();
    ECAL_API std::string       GetUdpMulticastGroup                 ();
    ECAL_API std::string       GetUdpMulticastMask                  ();
    ECAL_API int               GetUdpMulticastPort                  ();
    ECAL_API int               GetUdpMulticastTtl                   ();

    ECAL_API int               GetUdpMulticastSndBufSizeBytes       ();
    ECAL_API int               GetUdpMulticastRcvBufSizeBytes       ();

    ECAL_API bool              IsUdpMulticastJoinAllIfEnabled       ();

    ECAL_API int               GetMaxUdpBandwidthBytesPerSecond     ();

    ECAL_API bool              IsUdpMulticastRecEnabled             ();

    ECAL_API bool              IsNpcapEnabled                       ();

    ECAL_API std::string       GetHostGroupName                     ();

    /////////////////////////////////////
    // time
    /////////////////////////////////////

    ECAL_API std::string       GetTimesyncModuleName                ();

    /////////////////////////////////////
    // logging
    /////////////////////////////////////

    ECAL_API eCAL_Logging_Filter GetConsoleLogFilter                ();
    ECAL_API eCAL_Logging_Filter GetFileLogFilter                   ();

    /////////////////////////////////////
    // publisher
    /////////////////////////////////////

    ECAL_API bool              IsTopicTypeSharingEnabled            ();
    ECAL_API bool              IsTopicDescriptionSharingEnabled     ();

    /////////////////////////////////////
    // subscriber
    /////////////////////////////////////
    ECAL_API bool              GetDropOutOfOrderMessages            ();
  }
}
//@}
