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
 * @file   ecal_nanopb_common.cpp
 * @brief  eCAL common (de)serialization - nanopb variant
**/

#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"

#include <string>
#include <vector>

bool encode_string(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg)
  {
    if (arg == nullptr)  return false;
    if (*arg == nullptr) return false;

    char* str = (char*)(*arg);
    if (!pb_encode_tag_for_field(stream, field))
      return false;

    return pb_encode_string(stream, (pb_byte_t*)str, strlen(str));
  }

  struct SNanoBytes
  {
    pb_byte_t* content = nullptr;
    size_t     length  = 0;
  };
  bool encode_bytes(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg)
  {
    if (arg == nullptr)  return false;
    if (*arg == nullptr) return false;

    SNanoBytes* bytes = (SNanoBytes*)(*arg);
    if (!pb_encode_tag_for_field(stream, field))
      return false;

    return pb_encode_string(stream, (pb_byte_t*)bytes->content, bytes->length);
  }

  bool decode_string(pb_istream_t* stream, const pb_field_t* field, void** arg)
  {
    if (arg == nullptr)  return false;
    if (*arg == nullptr) return false;

    size_t len = stream->bytes_left;
    auto trg_string = (std::string*)(*arg);
    trg_string->resize(len);

    if (!pb_read(stream, (pb_byte_t*)(trg_string->data()), trg_string->size()))
      return false;

    return true;
  }

  bool decode_payload(pb_istream_t* stream, const pb_field_t* field, void** arg)
  {
    if (arg == nullptr)  return false;
    if (*arg == nullptr) return false;

    size_t len = stream->bytes_left;
    auto trg_vector = (std::vector<char>*)(*arg);
    trg_vector->resize(len);

    if (!pb_read(stream, (pb_byte_t*)(trg_vector->data()), trg_vector->size()))
      return false;

    return true;
  }
