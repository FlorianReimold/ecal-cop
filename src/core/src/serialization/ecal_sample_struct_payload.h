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
 * @file   ecal_sample_struct_payload.h
 * @brief  eCAL payload as struct
**/

#pragma once

#include "ecal_sample_struct_common.h"

#include <map>
#include <string>
#include <vector>

namespace eCAL
{
  namespace Payload
  {
    // Transport layer information
    struct TLayer
    {
      eTLayerType                         type      = tl_none;          // transport layer type
      int32_t                             version   = 0;                // transport layer version
      bool                                confirmed = false;            // transport layer used?
    };

    // Topic information
    struct Topic
    {
      std::string                         hname;                        // host name
      std::string                         tid;                          // topic id
      std::string                         tname;                        // topic name
      std::vector<TLayer>                 tlayer;                       // active topic transport layers and its specific parameter
    };

    // Topic content
    struct Content
    {
      int64_t                             id    = 0;                    // payload id
      int64_t                             clock = 0;                    // internal used clock
      int64_t                             time  = 0;                    // time the content was updated
      int64_t                             hash  = 0;                    // unique hash for that payload
      const char*                         payload_snd_ptr = nullptr;    // payload address
      size_t                              payload_snd_size = 0;         // payload size
      std::vector<char>                   payload_rec_vec;              // octet stream
    };

    // Payload sample
    struct Sample
    {
      eCmdType                            cmd_type = {};                // payload command type
      Topic                               topic;                        // topic information
      Content                             content;                      // topic content
    };
  }
}
