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

#include "nanopb/ecal.pb.h"
#include "nanopb/pb_encode.h"
#include "nanopb/pb_decode.h"

#include <string>
#include <vector>

#include "ecal_sample_nanopb_common.h"

namespace eCAL
{
  namespace nanopb
  {
    ///////////////////////////////////////////////
    // string
    ///////////////////////////////////////////////
    bool encode_string_field(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg)
    {
      if (arg == nullptr)  return false;
      if (*arg == nullptr) return false;

      if (!pb_encode_tag_for_field(stream, field))
        return false;

      char* str = (char*)(*arg);
      return pb_encode_string(stream, (pb_byte_t*)str, strlen(str));
    }

    void encode_string(pb_callback_t& pb_callback, const std::string& str)
    {
      if (str.size() == 0) return;

      pb_callback.funcs.encode = &encode_string_field;
      pb_callback.arg = (void*)(str.c_str());
    }

    bool decode_string_field(pb_istream_t* stream, const pb_field_t* field, void** arg)
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

    void decode_string(pb_callback_t& pb_callback, std::string& str)
    {
      pb_callback.funcs.decode = &decode_string_field;
      pb_callback.arg = &str;
    }

    ///////////////////////////////////////////////
    // bytes
    ///////////////////////////////////////////////
    bool encode_bytes_field(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg)
    {
      if (arg == nullptr)  return false;
      if (*arg == nullptr) return false;

      if (!pb_encode_tag_for_field(stream, field))
        return false;

      SNanoBytes* bytes = (SNanoBytes*)(*arg);
      return pb_encode_string(stream, (pb_byte_t*)bytes->content, bytes->length);
    }

    void encode_bytes(pb_callback_t& pb_callback, const SNanoBytes& nano_bytes)
    {
      if (nano_bytes.length == 0) return;

      pb_callback.funcs.encode = &encode_bytes_field;
      pb_callback.arg = (void*)(&nano_bytes);
    }

    bool decode_bytes_field(pb_istream_t* stream, const pb_field_t* field, void** arg)
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

    void decode_bytes(pb_callback_t& pb_callback, std::vector<char>& vec)
    {
      pb_callback.funcs.decode = &decode_bytes_field;
      pb_callback.arg = &vec;
    }

    ///////////////////////////////////////////////
    // payload_layer
    ///////////////////////////////////////////////
    bool encode_payload_layer_field(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg)
    {
      if (arg == nullptr)  return false;
      if (*arg == nullptr) return false;

      std::vector<eCAL::Payload::TLayer>* layer_vec = (std::vector<eCAL::Payload::TLayer>*)(*arg);

      for (auto layer : *layer_vec)
      {
        if (!pb_encode_tag_for_field(stream, field))
        {
          return false;
        }

        eCAL_pb_TLayer pb_layer = eCAL_pb_TLayer_init_default;
        pb_layer.type      = static_cast<eCAL_pb_eTLayerType>(layer.type);
        pb_layer.version   = layer.version;
        pb_layer.confirmed = layer.confirmed;

        //pb_layer.has_par_layer = true;
        //pb_layer.par_layer     = layer.par_layer;

        if (!pb_encode_submessage(stream, eCAL_pb_TLayer_fields, &pb_layer))
        {
          return false;
        }
      }

      return true;
    }

    void encode_payload_layer(pb_callback_t& pb_callback, const std::vector<eCAL::Payload::TLayer>& layer_vec)
    {
      pb_callback.funcs.encode = &encode_payload_layer_field;
      pb_callback.arg = (void*)(&layer_vec);
    }

    bool decode_payload_layer_field(pb_istream_t* stream, const pb_field_t* field, void** arg)
    {
      if (arg == nullptr)  return false;
      if (*arg == nullptr) return false;

      eCAL_pb_TLayer pb_layer = eCAL_pb_TLayer_init_default;

      if (!pb_decode(stream, eCAL_pb_TLayer_fields, &pb_layer))
      {
        return false;
      }

      auto trg_vector = (std::vector<eCAL::Payload::TLayer>*)(*arg);

      eCAL::Payload::TLayer layer;
      layer.type      = static_cast<eCAL::eTLayerType>(pb_layer.type);
      layer.version   = pb_layer.version;
      layer.confirmed = pb_layer.confirmed;

      trg_vector->push_back(layer);

      return true;
    }

    void decode_payload_layer(pb_callback_t& pb_callback, std::vector<eCAL::Payload::TLayer>& layer_vec)
    {
      pb_callback.funcs.decode = &decode_payload_layer_field;
      pb_callback.arg = &layer_vec;
    }

    ///////////////////////////////////////////////
    // registration_layer
    ///////////////////////////////////////////////
    bool encode_registration_layer_field(pb_ostream_t* stream, const pb_field_iter_t* field, void* const* arg)
    {
      if (arg == nullptr)  return false;
      if (*arg == nullptr) return false;

      std::vector<eCAL::Registration::TLayer>* layer_vec = (std::vector<eCAL::Registration::TLayer>*)(*arg);

      for (auto layer : *layer_vec)
      {
        if (!pb_encode_tag_for_field(stream, field))
        {
          return false;
        }

        eCAL_pb_TLayer pb_layer = eCAL_pb_TLayer_init_default;
        pb_layer.type      = static_cast<eCAL_pb_eTLayerType>(layer.type);
        pb_layer.version   = layer.version;
        pb_layer.confirmed = layer.confirmed;

        //pb_layer.has_par_layer = true;
        //pb_layer.par_layer     = layer.par_layer;

        if (!pb_encode_submessage(stream, eCAL_pb_TLayer_fields, &pb_layer))
        {
          return false;
        }
      }

      return true;
    }

    void encode_registration_layer(pb_callback_t& pb_callback, const std::vector<eCAL::Registration::TLayer>& layer_vec)
    {
      pb_callback.funcs.encode = &encode_registration_layer_field;
      pb_callback.arg = (void*)(&layer_vec);
    }

    bool decode_registration_layer_field(pb_istream_t* stream, const pb_field_t* field, void** arg)
    {
      if (arg == nullptr)  return false;
      if (*arg == nullptr) return false;

      eCAL_pb_TLayer pb_layer = eCAL_pb_TLayer_init_default;

      if (!pb_decode(stream, eCAL_pb_TLayer_fields, &pb_layer))
      {
        return false;
      }

      auto trg_vector = (std::vector<eCAL::Registration::TLayer>*)(*arg);

      eCAL::Registration::TLayer layer;
      layer.type      = static_cast<eCAL::eTLayerType>(pb_layer.type);
      layer.version   = pb_layer.version;
      layer.confirmed = pb_layer.confirmed;
      //layer.par_layer = pb_layer.par_layer;

      trg_vector->push_back(layer);

      return true;
    }

    void decode_registration_layer(pb_callback_t& pb_callback, std::vector<eCAL::Registration::TLayer>& layer_vec)
    {
      pb_callback.funcs.decode = &decode_registration_layer_field;
      pb_callback.arg = &layer_vec;
    }
  }
}
