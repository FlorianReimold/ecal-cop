# ========================= eCAL LICENSE =================================
#
# Copyright (C) 2016 - 2019 Continental Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ========================= eCAL LICENSE =================================
cmake_minimum_required(VERSION 3.10)

project(eCAL_samples VERSION "${ECAL_VERSION_STRING}")

# --------------------------------------------------------
# c samples
# --------------------------------------------------------
# pubsub
add_subdirectory(c/pubsub/string/minimal_rec)
add_subdirectory(c/pubsub/string/minimal_rec_cb)
add_subdirectory(c/pubsub/string/minimal_snd)

# --------------------------------------------------------
# cpp samples
# --------------------------------------------------------
# benchmarks
if (ECAL_CORE_COMMAND_LINE)
add_subdirectory(cpp/benchmarks/latency_rec)
add_subdirectory(cpp/benchmarks/latency_snd)
endif()
add_subdirectory(cpp/benchmarks/performance_rec_cb)
add_subdirectory(cpp/benchmarks/performance_snd)

# pubsub
add_subdirectory(cpp/pubsub/binary/binary_rec)
add_subdirectory(cpp/pubsub/binary/binary_snd)
add_subdirectory(cpp/pubsub/binary/ping)
add_subdirectory(cpp/pubsub/binary/pong)
add_subdirectory(cpp/pubsub/protobuf/person_loopback)
add_subdirectory(cpp/pubsub/protobuf/person_rec)
add_subdirectory(cpp/pubsub/protobuf/person_snd)
add_subdirectory(cpp/pubsub/string/minimal_rec)
add_subdirectory(cpp/pubsub/string/minimal_rec_cb)
add_subdirectory(cpp/pubsub/string/minimal_snd)
