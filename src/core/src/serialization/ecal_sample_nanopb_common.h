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
 * @file   ecal_nanopb_common.h
 * @brief  eCAL common (de)serialization - nanopb variant
**/

#pragma once

#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"

bool encode_string(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg);
struct SNanoBytes
{
  pb_byte_t* content = nullptr;
  size_t     length  = 0;
};
bool encode_bytes(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg);

bool decode_string(pb_istream_t* stream, const pb_field_t* field, void** arg);
bool decode_payload(pb_istream_t* stream, const pb_field_t* field, void** arg);
