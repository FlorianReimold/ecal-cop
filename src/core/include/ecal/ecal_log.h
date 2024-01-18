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
 * @file   ecal_log.h
 * @brief  eCAL logging interface
**/

#pragma once

#include <ecal/ecal_os.h>
#include <ecal/ecal_log_level.h>

#include <list>
#include <string>

namespace eCAL
{  
  namespace Logging
  {
    /**
     * @brief Sets the log level.
     *
     * @param level_  The level.
    **/
    ECAL_API void SetLogLevel(eCAL_Logging_eLogLevel level_);

    /**
     * @brief Get the current log level.
     *
     * @return   The current log level.
    **/
    ECAL_API eCAL_Logging_eLogLevel GetLogLevel();

    /**
     * @brief Log a message (with current log level).
     *
     * @param msg_  The log message string.
    **/
    ECAL_API void Log(const std::string& msg_);

    /**
     * @brief Log a message.
     *
     * @param level_  The level.
     * @param msg_    The log message string.
    **/
    inline void Log(eCAL_Logging_eLogLevel level_, const std::string& msg_)
    {
      SetLogLevel(level_);
      Log(msg_);
    }

    /**
     * @struct LogMessage
    **/
    struct SLogMessage
    {
      int64_t                time   = 0;               ///< Receive time of the log message.
      std::string            hname;                    ///< Sender host name.
      int32_t                pid    = 0;               ///< Sender process id.
      std::string            pname;                    ///< Sender process name.
      std::string            uname;                    ///< Sender unit name.
      eCAL_Logging_eLogLevel level  = log_level_none;  ///< Log message level.
      std::string            content;                  ///< Log message content.
    };

    /**
     * @brief Get complete logging as log message list. 
     *
     * @param [out] msg_list_  List to store the logging messages. 
    **/
    ECAL_API void GetLogging(std::list<SLogMessage>& msg_list_);
  }
}
