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
* @brief  eCAL process interface
**/

#include <ecal/ecal.h>
#include <ecal/ecal_config.h>

#include "ecal_def.h"
#include "config/ecal_config_reader_hlp.h"
#include "registration/ecal_registration_receiver.h"
#include "ecal_globals.h"
#include "ecal_process.h"
#include "io/udp/ecal_udp_configurations.h"

#include <array>
#include <thread>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <string>

#ifdef ECAL_OS_WINDOWS
#include "ecal_win_main.h"
#include <iphlpapi.h>
#include <ecal_utils/str_convert.h>
#endif /* ECAL_OS_WINDOWS */

#ifdef ECAL_OS_LINUX
#include <spawn.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/select.h>
#include <limits.h>
#include <netinet/in.h>

#include <ecal_utils/ecal_utils.h>

#endif /* ECAL_OS_LINUX */

#ifdef ECAL_OS_MACOS
#include <mach-o/dyld.h>
#include <sys/sysctl.h>
#endif // ECAL_OS_MACOS

#ifdef ECAL_OS_FREEBSD
#include <sys/user.h>
#include <sys/sysctl.h>
#include <libutil.h>
#endif

#ifdef ECAL_NPCAP_SUPPORT
#include <udpcap/npcap_helpers.h>
#endif // ECAL_NPCAP_SUPPORT

#include <ecal_utils/command_line.h>

#ifndef NDEBUG
#define STD_COUT_DEBUG( x ) { std::stringstream ss; ss << x; std::cout << ss.str(); }
#else
#define STD_COUT_DEBUG( x )
#endif

namespace
{
  std::string GetBufferStr(int size)
  {
    std::string unit = "byte";
    if (size > 1024)
    {
      size /= 1024;
      unit = "kByte";
    }
    if (size > 1024)
    {
      size /= 1024;
      unit = "MByte";
    }
    return std::to_string(size) + " " + unit;
  }

  std::string LayerMode(int mode)
  {
    switch (mode)
    {
    case 0:
      return "off";
      break;
    case 1:
      return "on";
      break;
    case 2:
      return "auto";
      break;
    }
    return "???";
  }
#if defined(ECAL_OS_WINDOWS)
  std::pair<bool, int> get_host_id()
  {
    // retrieve needed buffer size for GetAdaptersInfo
    ULONG alloc_adapters_size(0);
    {
      IP_ADAPTER_INFO AdapterInfo;
      GetAdaptersInfo(&AdapterInfo, &alloc_adapters_size);
    }
    if(alloc_adapters_size == 0) return std::make_pair(false, 0);

    // allocate adapter memory
    auto adapter_mem = std::make_unique<char[]>(static_cast<size_t>(alloc_adapters_size));

    // get all adapter infos
    PIP_ADAPTER_INFO pAdapter(nullptr);
    pAdapter = reinterpret_cast<PIP_ADAPTER_INFO>(adapter_mem.get());
    const DWORD dwStatus = GetAdaptersInfo(pAdapter, &alloc_adapters_size);
    if (dwStatus != ERROR_SUCCESS) return std::make_pair(false, 0);

    // iterate adapters and create hash
    int hash(0);
    while(pAdapter != nullptr)
    {
      for (UINT i = 0; i < pAdapter->AddressLength; ++i)
      {
        hash += (pAdapter->Address[i] << ((i & 1) * 8));
      }
      pAdapter = pAdapter->Next;
    }

    // return success
    return std::make_pair(true, hash);
  }
#elif defined(ECAL_OS_QNX)
  std::pair<bool, int> get_host_id()
  {
    // TODO: Find a suitable method on QNX to calculate an unqiue host identifier
    return std::make_pair(true, -1);
  }
#else
  std::pair<bool, int> get_host_id()
  {
    return std::make_pair(true, static_cast<int>(gethostid()));
  }
#endif
}

namespace eCAL
{
  namespace Process
  {
    void DumpConfig()
    {
      std::string cfg;
      DumpConfig(cfg);
      std::cout << cfg;
    }

    void DumpConfig(std::string& cfg_s_)
    {
      std::stringstream sstream;
      sstream << "------------------------- SYSTEM ---------------------------------" << std::endl;
      sstream << "Version                  : " << ECAL_VERSION << " (" << ECAL_DATE << ")" << std::endl;
#ifdef ECAL_OS_WINDOWS
#ifdef _WIN64
      sstream << "Platform                 : x64" << std::endl;
#else
      sstream << "Platform                 : win32" << std::endl;
#endif
#endif
#ifdef ECAL_OS_LINUX
      sstream << "Platform                 : linux" << std::endl;
#endif
      sstream << std::endl;

      if (eCAL::IsInitialized() == 0)
      {
        sstream << "Components               : NOT INITIALIZED ( call eCAL::Initialize() )";
        sstream << std::endl;
        cfg_s_ = sstream.str();
        return;
      }

      sstream << "------------------------- CONFIGURATION --------------------------" << std::endl;
      sstream << "Default INI              : " << Config::GetLoadedEcalIniPath() << std::endl; 
      sstream << std::endl;

      sstream << "------------------------- NETWORK --------------------------------" << std::endl;
      sstream << "Host name                : " << Process::GetHostName() << std::endl;
      sstream << "Host group name          : " << Process::GetHostGroupName() << std::endl;

      if (Config::IsNetworkEnabled())
      {
        sstream << "Network mode             : cloud" << std::endl;
      }
      else
      {
        sstream << "Network mode             : local" << std::endl;
      }
      sstream << "Network ttl              : " << UDP::GetMulticastTtl() << std::endl;
      sstream << "Network sndbuf           : " << GetBufferStr(Config::GetUdpMulticastSndBufSizeBytes()) << std::endl;
      sstream << "Network rcvbuf           : " << GetBufferStr(Config::GetUdpMulticastRcvBufSizeBytes()) << std::endl;
      sstream << "Multicast cfg version    : v" << static_cast<uint32_t>(Config::GetUdpMulticastConfigVersion()) << std::endl;
      sstream << "Multicast group          : " << Config::GetUdpMulticastGroup() << std::endl;
      sstream << "Multicast mask           : " << Config::GetUdpMulticastMask() << std::endl;
      const int port = Config::GetUdpMulticastPort();
      sstream << "Multicast ports          : " << port << " - " << port + 10 << std::endl;
      sstream << "Multicast join all IFs   : " << (Config::IsUdpMulticastJoinAllIfEnabled() ? "on" : "off") << std::endl;
      auto bandwidth = Config::GetMaxUdpBandwidthBytesPerSecond();
      if (bandwidth < 0)
      {
        sstream << "Bandwidth limit (udp)    : not limited" << std::endl;
      }
      else
      {
        sstream << "Bandwidth limit udp      : " << GetBufferStr(bandwidth) + "/s" << std::endl;
      }
      sstream << std::endl;

      sstream << "------------------------- TIME -----------------------------------" << std::endl;
      sstream << "Synchronization realtime : " << Config::GetTimesyncModuleName() << std::endl;
      sstream << "Synchronization replay   : " << eCALPAR(TIME, SYNC_MOD_REPLAY) << std::endl;
      sstream << "State                    : ";
      if (g_timegate()->IsSynchronized()) sstream << " synchronized " << std::endl;
      else                                sstream << " not synchronized " << std::endl;
      sstream << "Master / Slave           : ";
      if (g_timegate()->IsMaster())       sstream << " Master " << std::endl;
      else                                sstream << " Slave " << std::endl;
      int         status_state;
      std::string status_msg;
      g_timegate()->GetStatus(status_state, &status_msg);
      sstream << "Status (Code)            : \"" << status_msg << "\" (" << status_state << ")" << std::endl;
      sstream << std::endl;

      sstream << "------------------------- SUBSCRIPTION LAYER DEFAULTS ------------" << std::endl;
      sstream << "Layer Mode UDP MC        : " << LayerMode(Config::IsUdpMulticastRecEnabled()) << std::endl;
      sstream << "Npcap UDP Reciever       : " << LayerMode(Config::IsNpcapEnabled());
#ifdef ECAL_NPCAP_SUPPORT
      if(Config::IsNpcapEnabled() && !Udpcap::Initialize())
      {
        sstream << " (Init FAILED!)";
      }
#else  // ECAL_NPCAP_SUPPORT
      if (Config::IsNpcapEnabled())
      {
        sstream << " (Npcap is enabled, but not configured via CMake!)";
      }
#endif // ECAL_NPCAP_SUPPORT
      sstream << std::endl;
      sstream << "Drop out-of-order msgs   : " << (Config::GetDropOutOfOrderMessages() ? "on" : "off") << std::endl;
      sstream << std::endl;

      // write it into std:string
      cfg_s_ = sstream.str();
    }

    std::string GetHostName()
    {
      if (g_host_name.empty())
      {
        char hname[1024] = { 0 };
        if (gethostname(hname, 1024) == 0)
        {
          g_host_name = hname;
        }
        else
        {
          std::cerr << "Unable to get host name" << std::endl;
        }
      }
      return(g_host_name);
    }

    std::string GetHostGroupName()
    {
      return Config::GetHostGroupName().empty() ? GetHostName() : Config::GetHostGroupName();
    }

    int GetHostID()
    {
      return internal::GetHostID();
    }

    namespace internal
    {
      int GetHostID()
      {
        if (g_host_id == 0)
        {
          // try to get unique host id
          bool success(false);
          int  id(0);
          std::tie(success, id) = get_host_id();
          if (success)
          {
            g_host_id = id;
          }
          // never try again to not waste time
          else
          {
            g_host_id = -1;
            std::cerr << "Unable to get host id" << std::endl;
          }
        }
        return(g_host_id);
      }
    }

    std::string GetUnitName()
    {
      return(g_unit_name);
    }

    std::string GetTaskParameter(const char* sep_)
    {
      std::string par_line;
      for (auto par : g_task_parameter)
      {
        if (!par_line.empty()) par_line += sep_;
        par_line += par;
      }
      return(par_line);
    }

    void SleepMS(const long time_ms_)
    {
      #ifdef ECAL_OS_WINDOWS
      {
        Sleep(time_ms_);
      }
      #else
        std::this_thread::sleep_for(std::chrono::milliseconds(time_ms_));
      #endif
    }

    void SleepNS(const long long time_ns_)
    {
      #ifdef ECAL_OS_WINDOWS
      {
        auto milliseconds = time_ns_ / 1000000 + ((time_ns_ % 1000000) != 0);
        Sleep(static_cast<DWORD>(milliseconds));
      }
      #else
        std::this_thread::sleep_for(std::chrono::nanoseconds(time_ns_));
      #endif
    }

    void SetState(eCAL_Process_eSeverity severity_, eCAL_Process_eSeverity_Level level_, const char* info_)
    {
      g_process_severity = severity_;
      g_process_severity_level = level_;
      if (info_ != nullptr)
      {
        g_process_info = info_;
      }
    }

    int AddRegistrationCallback(enum eCAL_Registration_Event event_, RegistrationCallbackT callback_)
    {
#if ECAL_CORE_REGISTRATION
      if (g_registration_receiver() == nullptr) return -1;
      if (g_registration_receiver()->AddRegistrationCallback(event_, callback_)) return 0;
#endif
      return -1;
    }

    int RemRegistrationCallback(enum eCAL_Registration_Event event_)
    {
#if ECAL_CORE_REGISTRATION
      if (g_registration_receiver() == nullptr) return -1;
      if (g_registration_receiver()->RemRegistrationCallback(event_)) return 0;
#endif
      return -1;
    }
  }
}


#ifdef ECAL_OS_WINDOWS

namespace
{
  void create_proc_id()
  {
    if (eCAL::g_process_id == 0)
    {
      eCAL::g_process_id   = GetCurrentProcessId();
      eCAL::g_process_id_s = std::to_string(eCAL::g_process_id);
    }
  }
}

namespace eCAL
{
  namespace Process
  {
    int GetProcessID()
    {
      create_proc_id();
      return(g_process_id);
    }

    std::string GetProcessIDAsString()
    {
      create_proc_id();
      return(g_process_id_s);
    }

    std::string GetProcessName()
    {
      if (g_process_name.empty())
      {
        WCHAR pname[1024] = { 0 };
        GetModuleFileNameExW(GetCurrentProcess(), 0, pname, 1024);
        g_process_name = EcalUtils::StrConvert::WideToUtf8(pname);
      }
      return(g_process_name);
    }

    std::string GetProcessParameter()
    {
      if (g_process_par.empty())
      {
        g_process_par = EcalUtils::CommandLine::GetUtf8CommandLine();
      }
      return(g_process_par);
    }
  }
}

#endif /* ECAL_OS_WINDOWS */


#ifdef ECAL_OS_LINUX

extern char **environ;

namespace
{
  void create_proc_id()
  {
    if (eCAL::g_process_id == 0)
    {
      eCAL::g_process_id   = getpid();
      eCAL::g_process_id_s = std::to_string(eCAL::g_process_id);
    }
  }

  int parseLine(char* line)
  {
    int i = strlen(line);
    while (*line < '0' || *line > '9') line++;
    line[i - 3] = '\0';
    i = atoi(line);
    return i;
  }
}

namespace eCAL
{
  namespace Process
  {
    int GetProcessID()
    {
      create_proc_id();
      return(g_process_id);
    }

    std::string GetProcessIDAsString()
    {
      create_proc_id();
      return(g_process_id_s);
    }

    /**
    * @brief Returns the fully qualified path for the current process's binary
    *
    * @return the process path
    */
    std::string GetProcessName()
    {
      if (g_process_name.empty()) {
        // Read the link to our own executable
        char buf[PATH_MAX] = { 0 };
#if defined(ECAL_OS_MACOS)
        uint32_t length = PATH_MAX;
        if (_NSGetExecutablePath(buf, &length) != 0)
        {
          // Buffer size is too small.
          return "";
        }
        length = strlen(buf);
#elif defined(ECAL_OS_QNX)
        size_t length {0};
        // TODO: Find a suitable method on QNX to retrieve current process name
#elif defined(ECAL_OS_FREEBSD)
        size_t length {0};
        struct kinfo_proc *proc = kinfo_getproc(getpid());
        if (proc)
        {
          strncpy(buf, proc->ki_comm, sizeof(buf));
          length = strlen(buf);
          free(proc);
        }
#else
        ssize_t length = readlink("/proc/self/exe", buf, PATH_MAX);

        if (length < 0)
        {
          std::cerr << "Unable to get process name: " << strerror(errno) << std::endl;
          return "";
        }
#endif
        // Copy the binary name to a std::string
        g_process_name = std::string(buf, length);

      }
      return g_process_name;
    }
    std::string GetProcessParameter()
    {
      if (g_process_par.empty())
      {
#if defined(ECAL_OS_MACOS)
        int pid = getpid();

        int    mib[3], argmax, argc;
        size_t    size;

        mib[0] = CTL_KERN;
        mib[1] = KERN_ARGMAX;

        size = sizeof(argmax);
        if (sysctl(mib, 2, &argmax, &size, NULL, 0) == -1)
        {
          return "";
        }

        /* Allocate space for the arguments. */
        std::vector<char> procargs(argmax);

        /*
         * Make a sysctl() call to get the raw argument space of the process.
         * The layout is documented in start.s, which is part of the Csu
         * project.  In summary, it looks like:
         *
         * /---------------\ 0x00000000
         * :               :
         * :               :
         * |---------------|
         * | argc          |
         * |---------------|
         * | arg[0]        |
         * |---------------|
         * :               :
         * :               :
         * |---------------|
         * | arg[argc - 1] |
         * |---------------|
         * | 0             |
         * |---------------|
         * | env[0]        |
         * |---------------|
         * :               :
         * :               :
         * |---------------|
         * | env[n]        |
         * |---------------|
         * | 0             |
         * |---------------| <-- Beginning of data returned by sysctl() is here.
         * | argc          |
         * |---------------|
         * | exec_path     |
         * |:::::::::::::::|
         * |               |
         * | String area.  |
         * |               |
         * |---------------| <-- Top of stack.
         * :               :
         * :               :
         * \---------------/ 0xffffffff
         */
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROCARGS2;
        mib[2] = pid;

        size = (size_t)argmax;
        if (sysctl(mib, 3, procargs.data(), &size, NULL, 0) == -1)
        {
          return "";
        }

        // First few bytes are the argc
        memcpy(&argc, procargs.data(), sizeof(argc));
        size_t pos = sizeof(argc);

        if (argc == 0)
          return "";

        // Skip the saved exec_path
        for (; pos <= procargs.size(); pos++)
        {
          if (procargs[pos] == '\0')
          {
            // End of exec_path reached
            break;
          }
        }
        if (pos >= procargs.size())
          return "";

        // Skip trailing '\0' characters
        for (; pos <= procargs.size(); pos++)
        {
          if (procargs[pos] != '\0') {
            // Beginning of first argument reached
            break;
          }
        }
        if (pos >= procargs.size())
          return "";

        // Iterate through the '\0' terminated strings and copy them to a C++ vector
        std::vector<std::string> argument_vector;
        argument_vector.reserve(argc);

        size_t current_arg_start = pos;
        int current_arg_number = 0;
        for (size_t i = pos; i < procargs.size(); ++i)
        {
          if (procargs[i] == '\0')
          {
            std::string arg = std::string(&procargs[current_arg_start], i - current_arg_start);
            argument_vector.push_back(arg);
            current_arg_start = i + 1;
            current_arg_number++;
          }

          if (current_arg_number == argc)
            break;
        }

#elif defined(ECAL_OS_QNX)
        // TODO: Find a suitable method on QNX to get process arguments of the current executable
        std::vector<std::string> argument_vector;
        g_process_par = "";
#elif defined(ECAL_OS_FREEBSD)
        // create mib structure for sysctl call
        int mib[4];
        mib[0] = CTL_KERN;
        mib[1] = KERN_PROC;
        mib[2] = KERN_PROC_ARGS;
        mib[3] = getpid();

        // get the length of the return value buffer
        size_t len {0};
        sysctl(mib, sizeof mib , NULL, &len, NULL, 0);

        // retrieve process arguments as null-character seperated c-string
        auto buffer = std::make_unique<char[]>(len);
        sysctl(mib, sizeof mib, buffer.get(), &len, NULL, 0);
        size_t position = 0;
        
        // convert null-character seperated c-string to std::vector<std::string>
        std::vector<std::string> argument_vector;
        while(position < len)
        {
          const char* arg = &(buffer.get()[position]);
          argument_vector.push_back(arg);
          position += strlen(arg) + 1;
        }
#else

        const std::string filename = "/proc/self/cmdline";
        std::vector<std::string> argument_vector;

        std::ifstream cmdline_file(filename, std::ios::binary);
        if (!cmdline_file.is_open())
        {
          std::cerr << "Failed to open " << filename << '\n';
          return "";
        }
        else
        {
          std::string arg;
          while (std::getline(cmdline_file, arg, '\0')) // the cmdline contains arguments separated by \0
          {
            argument_vector.emplace_back(arg);
          }
        }

#endif // ECAL_OS_MACOS

        size_t complete_char_num(0);
        for (std::string& argument : argument_vector)
        {
          std::string escaped_arg;
          escaped_arg.reserve(argument.size() + 2);

          bool constains_space = (argument.find(' ') != std::string::npos);

          // Escape special characters
          if (constains_space) escaped_arg += '\"';
          for (char c : argument)
          {
            if (c == '\\')                              // Escape [\]
              escaped_arg += "\\\\";
            else if (c == '\"')                         // Escape ["]
              escaped_arg += "\\\"";
            else if (c == '\'')                         // Escape [']
              escaped_arg += "\\\'";
            else
              escaped_arg += c;
          }
          if (constains_space) escaped_arg += '\"';

          if(escaped_arg.empty())
            escaped_arg = "\"\"";

          complete_char_num += escaped_arg.size();
          argument = escaped_arg;
        }

        std::string process_par;
        process_par.reserve(complete_char_num + argument_vector.size());

        for (auto arg_it = argument_vector.begin(); arg_it != argument_vector.end(); arg_it++)
        {
          if (arg_it != argument_vector.begin())
            process_par += ' ';
          process_par += *arg_it;
        }

        g_process_par = process_par;
      }
      return(g_process_par);
    }
  }
}

#endif /* ECAL_OS_LINUX */

#undef STD_COUT_DEBUG
