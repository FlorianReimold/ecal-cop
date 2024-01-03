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

project(core VERSION ${eCAL_VERSION_STRING})

find_package(asio       REQUIRED)
if (ECAL_CORE_CONFIG_INIFILE)
find_package(simpleini  REQUIRED)
endif()
if (ECAL_CORE_COMMAND_LINE)
find_package(tclap      REQUIRED)
endif()
if (ECAL_NPCAP_SUPPORT)
  find_package(udpcap   REQUIRED)
endif()

# If we're currently doing a build within a git repository, we will configure the header files.
# Else, (e.g. for source packages such as debian source packages) we will use a preconfigured file.
# If there is really no information available, it will generate a dummy version file 0.0.0
if     (IS_GIT_TREE OR NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/ecal/ecal_defs.h")
  configure_file(src/ecal_defs.h.in "${CMAKE_CURRENT_BINARY_DIR}/include/ecal/ecal_defs.h" @ONLY)
endif  (IS_GIT_TREE OR NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/ecal/ecal_defs.h")

######################################
# config
######################################
set(ecal_config_src
    src/config/ecal_config.cpp
    src/config/ecal_config_reader.cpp
    src/config/ecal_config_reader.h
    src/config/ecal_config_reader_hlp.h
)

######################################
# io/udp
######################################
# io/udp/fragmentation
set(ecal_io_udp_fragmentation_src
    src/io/udp/fragmentation/msg_type.h
    src/io/udp/fragmentation/rcv_fragments.cpp
    src/io/udp/fragmentation/rcv_fragments.h
    src/io/udp/fragmentation/snd_fragments.cpp
    src/io/udp/fragmentation/snd_fragments.h
)

# io/udp/sendreceive (npcap)
if(ECAL_NPCAP_SUPPORT)
set(ecal_io_udp_sendreceive_src_npcap 
   src/io/udp/sendreceive/udp_receiver_npcap.cpp
   src/io/udp/sendreceive/udp_receiver_npcap.h
)
endif()

# io/udp/sendreceive
set(ecal_io_udp_sendreceive_src
    src/io/udp/sendreceive/udp_receiver.cpp
    src/io/udp/sendreceive/udp_receiver.h
    src/io/udp/sendreceive/udp_receiver_asio.cpp
    src/io/udp/sendreceive/udp_receiver_asio.h
    src/io/udp/sendreceive/udp_sender.cpp
    src/io/udp/sendreceive/udp_sender.h
    ${ecal_io_udp_sendreceive_src_npcap}
)

# io/udp/sendreceive/linux
if(UNIX)
  set(ecal_io_udp_sendreceive_linux_src
      src/io/udp/sendreceive/linux/socket_os.h
)
endif()

# io/udp/sendreceive/win32
if (WIN32)
  set(ecal_io_udp_sendreceive_win_src
      src/io/udp/sendreceive/win32/socket_os.h
)
endif()

# io/udp
set(ecal_io_udp_src
    src/io/udp/ecal_udp_configurations.cpp
    src/io/udp/ecal_udp_configurations.h
    src/io/udp/ecal_udp_sample_receiver.cpp
    src/io/udp/ecal_udp_sample_receiver.h
    src/io/udp/ecal_udp_sample_sender.cpp
    src/io/udp/ecal_udp_sample_sender.h
    src/io/udp/ecal_udp_topic2mcast.h
)

######################################
# logging
######################################
set(ecal_logging_src
    src/logging/ecal_log.cpp
    src/logging/ecal_log_impl.cpp
    src/logging/ecal_log_impl.h
)

######################################
# pubsub
######################################
set(ecal_pub_src
    src/pubsub/ecal_publisher.cpp
)
if(ECAL_CORE_PUBLISHER)
  list(APPEND ecal_pub_src
      src/pubsub/ecal_pubgate.cpp
      src/pubsub/ecal_pubgate.h
  )
endif()

set(ecal_sub_src
    src/pubsub/ecal_subscriber.cpp
)
if(ECAL_CORE_SUBSCRIBER)
  list(APPEND ecal_sub_src
      src/pubsub/ecal_subgate.cpp
      src/pubsub/ecal_subgate.h
  )
endif()

######################################
# readwrite
######################################
if(ECAL_CORE_PUBLISHER)
  set(ecal_writer_src
      src/readwrite/ecal_writer.cpp
      src/readwrite/ecal_writer.h
      src/readwrite/ecal_writer_base.h
      src/readwrite/ecal_writer_buffer_payload.h
      src/readwrite/ecal_writer_data.h
      src/readwrite/ecal_writer_info.h
  )
  set(ecal_writer_udp_src
      src/readwrite/udp/ecal_writer_udp_mc.cpp
      src/readwrite/udp/ecal_writer_udp_mc.h
  )
endif()

if(ECAL_CORE_SUBSCRIBER)
  set(ecal_reader_src
      src/readwrite/ecal_reader.cpp
      src/readwrite/ecal_reader.h
      src/readwrite/ecal_reader_layer.h
  )
  set(ecal_reader_udp_src
      src/readwrite/udp/ecal_reader_udp_mc.cpp
      src/readwrite/udp/ecal_reader_udp_mc.h
  )
endif()

######################################
# registration
######################################
if (ECAL_CORE_REGISTRATION)
  set(ecal_registration_src
      src/registration/ecal_registration_provider.cpp
      src/registration/ecal_registration_provider.h
      src/registration/ecal_registration_receiver.cpp
      src/registration/ecal_registration_receiver.h
  )
endif()

######################################
# serialization
######################################
set(ecal_serialization_src
    src/serialization/ecal_sample.h
	src/serialization/ecal_sample_struct_common.h
    src/serialization/ecal_sample_struct_payload.h
    src/serialization/ecal_sample_struct_registration.h
)
if(ECAL_CORE_PROTOBUF)
  list(APPEND ecal_serialization_src
      src/serialization/ecal_sample_protobuf_payload.cpp
      src/serialization/ecal_sample_protobuf_registration.cpp
  )
endif()
if(ECAL_CORE_NANOPB)
  list(APPEND ecal_serialization_src
      src/serialization/nanopb/ecal.pb.c
      src/serialization/nanopb/ecal.pb.h
      src/serialization/nanopb/host.pb.c
      src/serialization/nanopb/host.pb.h
      src/serialization/nanopb/layer.pb.c
      src/serialization/nanopb/layer.pb.h
      src/serialization/nanopb/process.pb.c
      src/serialization/nanopb/process.pb.h
      src/serialization/nanopb/topic.pb.c
      src/serialization/nanopb/topic.pb.h
      src/serialization/nanopb/pb.h
      src/serialization/nanopb/pb_common.c
      src/serialization/nanopb/pb_common.h
      src/serialization/nanopb/pb_decode.c
      src/serialization/nanopb/pb_decode.h
      src/serialization/nanopb/pb_encode.c
      src/serialization/nanopb/pb_encode.h      
      src/serialization/ecal_sample_nanopb_common.h
      src/serialization/ecal_sample_nanopb_common.cpp
      src/serialization/ecal_sample_nanopb_payload.cpp
      src/serialization/ecal_sample_nanopb_registration.cpp
  )
endif()
if(ECAL_CORE_CEREAL)
  list(APPEND ecal_serialization_src
      src/serialization/ecal_sample_cereal_payload.cpp
      src/serialization/ecal_sample_cereal_registration.cpp
  )
endif()

######################################
# time
######################################
set(ecal_time_src
    src/time/ecal_time.cpp
)
if(ECAL_CORE_TIMEPLUGIN)
  list(APPEND ecal_time_src
      src/time/ecal_timegate.cpp
      src/time/ecal_timegate.h
  )
endif()

######################################
# util
######################################
set(ecal_util_src
    src/util/ecal_expmap.h
    src/util/ecal_thread.h
    src/util/getenvvar.h
)
if (ECAL_CORE_COMMAND_LINE)
  list(APPEND ecal_util_src
      src/util/advanced_tclap_output.cpp
      src/util/advanced_tclap_output.h
  )
endif()


######################################
# common
######################################
set(ecal_cmn_src
    src/ecal.cpp
    src/ecal_c.cpp
    src/ecal_event.cpp 
    src/ecal_event.h
    src/ecal_eventhandle.h
    src/ecal_global_accessors.cpp
    src/ecal_globals.cpp
    src/ecal_process.cpp
    src/ecal_util.cpp
    src/ecal_def.h
    src/ecal_def_ini.h
    src/ecal_global_accessors.h
    src/ecal_globals.h
    src/ecal_sample_to_topicinfo.h
)
if (WIN32)
  list(APPEND ecal_cmn_src
      src/ecal_win_main.h
  )
endif()

######################################
# c interface
######################################
set(ecal_c_src
    src/ecal_c.cpp
)

######################################
# windows dll
######################################
if(WIN32)
  set(ecal_c_win_src
      src/win32/dll/dllmain.cpp
      src/win32/dll/ecal.rc
  )
endif()

######################################
# public header
######################################
set(ecal_header_cmn
    include/ecal/ecal.h
    include/ecal/ecal_c.h
    include/ecal/ecal_c_types.h
    include/ecal/ecal_callback.h
    include/ecal/ecal_config.h
    include/ecal/ecal_core.h
    include/ecal/ecal_deprecate.h
    include/ecal/ecal_init.h
    include/ecal/ecal_log.h
    include/ecal/ecal_log_level.h
    include/ecal/ecal_os.h
    include/ecal/ecal_payload_writer.h
    include/ecal/ecal_process.h
    include/ecal/ecal_process_severity.h
    include/ecal/ecal_publisher.h
    include/ecal/ecal_subscriber.h
    include/ecal/ecal_time.h
    include/ecal/ecal_util.h
    include/ecal/ecal_types.h
)

set(ecal_header_cimpl
    include/ecal/cimpl/ecal_callback_cimpl.h
    include/ecal/cimpl/ecal_core_cimpl.h
    include/ecal/cimpl/ecal_init_cimpl.h
    include/ecal/cimpl/ecal_log_cimpl.h
    include/ecal/cimpl/ecal_process_cimpl.h
    include/ecal/cimpl/ecal_publisher_cimpl.h
    include/ecal/cimpl/ecal_subscriber_cimpl.h
    include/ecal/cimpl/ecal_time_cimpl.h
    include/ecal/cimpl/ecal_util_cimpl.h
)

set(ecal_header_msg
    include/ecal/msg/protobuf/publisher.h
    include/ecal/msg/string/publisher.h
    include/ecal/msg/publisher.h
    include/ecal/msg/protobuf/subscriber.h
    include/ecal/msg/string/subscriber.h
    include/ecal/msg/subscriber.h
)

set(ecal_header_public
    ${ecal_header_cmn}
    ${ecal_header_cimpl}
    ${ecal_header_msg}
)

ecal_add_ecal_shared_library(${PROJECT_NAME} 
    ${ecal_config_src}
    ${ecal_io_udp_fragmentation_src}
    ${ecal_io_udp_sendreceive_src}
    ${ecal_io_udp_src}
    ${ecal_io_udp_sendreceive_linux_src}
    ${ecal_io_udp_sendreceive_win_src}
    ${ecal_logging_src} 
    ${ecal_pub_src} 
    ${ecal_sub_src} 
    ${ecal_writer_src} 
    ${ecal_writer_udp_src}
    ${ecal_reader_src} 
    ${ecal_reader_udp_src}
    ${ecal_registration_src}
    ${ecal_serialization_src}	
    ${ecal_time_src}
    ${ecal_util_src}
    ${ecal_cmn_src}
    ${ecal_header_public}
    ${CMAKE_CURRENT_BINARY_DIR}/include/ecal/ecal_defs.h
)

if(UNIX)
  set_source_files_properties(src/util/convert_utf.cpp PROPERTIES COMPILE_FLAGS -Wno-implicit-fallthrough)
endif()

ecal_add_ecal_shared_library(${PROJECT_NAME}_c ${ecal_c_src} ${ecal_c_win_src})

add_library(eCAL::${PROJECT_NAME}   ALIAS ${PROJECT_NAME})
add_library(eCAL::${PROJECT_NAME}_c ALIAS ${PROJECT_NAME}_c)

target_link_libraries(${PROJECT_NAME}_c ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}_c
  INTERFACE ECAL_C_DLL
  PUBLIC
    ASIO_STANDALONE
    ASIO_DISABLE_VISIBILITY
  PRIVATE 
    eCAL_EXPORTS
    ECAL_NO_DEPRECATION_WARNINGS
)

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    ASIO_STANDALONE
    ASIO_DISABLE_VISIBILITY
  PRIVATE
    eCAL_EXPORTS
    $<$<BOOL:${ECAL_HAS_CLOCKLOCK_MUTEX}>:ECAL_HAS_CLOCKLOCK_MUTEX>
    $<$<BOOL:${ECAL_HAS_ROBUST_MUTEX}>:ECAL_HAS_ROBUST_MUTEX>
    $<$<BOOL:${ECAL_USE_CLOCKLOCK_MUTEX}>:ECAL_USE_CLOCKLOCK_MUTEX>
    ECAL_NO_DEPRECATION_WARNINGS
)

if(ECAL_CORE_CONFIG_INIFILE)
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE ECAL_CORE_CONFIG_INIFILE)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      simpleini::simpleini
  )
endif()

if(ECAL_CORE_COMMAND_LINE)
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE ECAL_CORE_COMMAND_LINE)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      tclap::tclap
  )
endif()

if(ECAL_CORE_REGISTRATION)
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE ECAL_CORE_REGISTRATION)
  target_compile_definitions(${PROJECT_NAME}_c
    PRIVATE ECAL_CORE_REGISTRATION)
endif()

if(ECAL_CORE_PUBLISHER)
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE ECAL_CORE_PUBLISHER)
  target_compile_definitions(${PROJECT_NAME}_c
    PRIVATE ECAL_CORE_PUBLISHER)
endif()

if(ECAL_CORE_SUBSCRIBER)
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE ECAL_CORE_SUBSCRIBER)
  target_compile_definitions(${PROJECT_NAME}_c
    PRIVATE ECAL_CORE_SUBSCRIBER)
endif()

if(ECAL_CORE_TIMEPLUGIN)
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE ECAL_CORE_TIMEPLUGIN)
  target_compile_definitions(${PROJECT_NAME}_c
    PRIVATE ECAL_CORE_TIMEPLUGIN)
endif()

if(ECAL_CORE_PROTOBUF)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      eCAL::core_pb
  )
endif()

if(ECAL_NPCAP_SUPPORT)
  target_compile_definitions(${PROJECT_NAME}
    PRIVATE ECAL_NPCAP_SUPPORT)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      udpcap::udpcap
  )
endif()

target_include_directories(${PROJECT_NAME}
  PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE
    $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${QNXNTO}>>>:dl>
    $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>,$<NOT:$<BOOL:${QNXNTO}>>>:rt>
    $<$<STREQUAL:${CMAKE_SYSTEM_NAME},FreeBSD>:util>
    $<$<BOOL:${WIN32}>:iphlpapi>
    $<$<BOOL:${WIN32}>:psapi> 
    $<$<BOOL:${WIN32}>:shlwapi.lib> 
    $<$<BOOL:${WIN32}>:winmm> 
    $<$<BOOL:${WIN32}>:ws2_32> 
    $<$<BOOL:${WIN32}>:wsock32>
    $<$<BOOL:${QNXNTO}>:socket>
    asio::asio
    eCAL::ecal-utils
)

set_property(TARGET ${PROJECT_NAME}   PROPERTY FOLDER core)
set_property(TARGET ${PROJECT_NAME}_c PROPERTY FOLDER core)

ecal_install_ecal_shared_library(${PROJECT_NAME}_c)
ecal_install_ecal_shared_library(${PROJECT_NAME})

install(DIRECTORY
   "include/" DESTINATION "${INSTALL_INCLUDE_DIR}" COMPONENT sdk
    FILES_MATCHING PATTERN "*.h")

#Install generated ecal_defs.h file    
install(DIRECTORY
   "${CMAKE_CURRENT_BINARY_DIR}/include/" DESTINATION "${INSTALL_INCLUDE_DIR}" COMPONENT sdk
    FILES_MATCHING PATTERN "*.h")
    

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES 
    ${ecal_config_src}
    ${ecal_io_udp_fragmentation_src}
    ${ecal_io_udp_sendreceive_src}
    ${ecal_io_udp_src}
    ${ecal_io_udp_sendreceive_linux_src}
    ${ecal_io_udp_sendreceive_win_src}
    ${ecal_logging_src} 
    ${ecal_pub_src} 
    ${ecal_sub_src} 
    ${ecal_writer_src} 
    ${ecal_writer_udp_src}
    ${ecal_reader_src} 
    ${ecal_reader_udp_src}
    ${ecal_registration_src}   
    ${ecal_serialization_src}	
    ${ecal_time_src}
    ${ecal_util_src}
    ${ecal_cmn_src}
    ${ecal_c_src}
    ${ecal_c_win_src}
    ${ecal_header_public}
  )
endif()
