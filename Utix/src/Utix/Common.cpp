/*

UTIX - utility library from XChip
Copyright (C) 2016  Rafael Moura

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/gpl-3.0.html.

*/


#ifdef _WIN32
#include <stdlib.h>
#include <windows.h>
#elif defined(__linux__) 
#include <string.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#endif

#include <Utix/Log.h>
#include <Utix/Common.h>


namespace utix {



std::string GetFullProcName()
{
     
#ifdef _WIN32



     HMODULE handle = GetModuleHandleW(NULL);
     WCHAR buffer[MAX_PATH];
     auto writeSize = GetModuleFileNameW(handle, buffer, MAX_PATH);
     
     if (writeSize == MAX_PATH)
     {
          const auto errorCode = GetLastError();
          LogError("Error in GetModuleFileName ErrorCode: %d", errorCode);
          return std::string();

     }

     std::string ret;
     ret.resize(writeSize);
     std::wcstombs(&ret[0], buffer, writeSize);
     
     return ret;




#elif defined(__linux__)


     constexpr const size_t BUFF_LEN = 400;
     char buffer[BUFF_LEN];
     auto writeSize = readlink("/proc/self/exe", buffer, BUFF_LEN);

     if (writeSize == -1)
     {
          LogError("Error in readlink");
          return std::string();
     }

     buffer[writeSize] = 0;

     return buffer;




#elif defined(__APPLE__)



     constexpr const uint32_t BUFF_LEN = 400;
     
     char buffer[BUFF_LEN];
     uint32_t size = BUFF_LEN;

     if (_NSGetExecutablePath(buffer, &size) != 0)
     {
          LogError("_NSGetExecutablePath failed. output size: %d", size);
          return std::string();
     }
     
     return buffer;




#endif
     
}





std::string GetFullProcDir()
{
#ifdef _WIN32
     constexpr const char dirSlash = '\\';
#elif defined(__linux__) || defined(__APPLE__)
     constexpr const char dirSlash = '/';
#endif

     auto fullPath = GetFullProcName();
     fullPath.erase(fullPath.find_last_of(dirSlash), fullPath.size());
     
     return fullPath;
}



















	
}