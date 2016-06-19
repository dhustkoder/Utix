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

#ifndef _UTIX_ASSERT_H_
#define _UTIX_ASSERT_H_


#if !_DEBUG
// if not debug build, includes nothing.
#define NDEBUG 1
#define ASSERT_MSG(cond, msg)




#else

// else, includes...
#if defined(__linux__) || defined(__APPLE__)
#include <signal.h>
#endif

#include <cassert>
#include "Log.h"

#ifdef _WIN32
#define _UTIX_DEBUG_BREAK_() __debugbreak()
#elif defined(__linux__) || defined(__APPLE__)
#define _UTIX_DEBUG_BREAK_() raise(SIGTRAP)
#endif

#define _UTIX_MACRO_STR_EX_(x) #x
#define _UTIX_MACRO_STR_(x) _XCHIP_MACRO_STR_EX_(x)

#define ASSERT_MSG(cond, msg)                                            \
{                                                                        \
    using xchip::utils::LogError;                                        \
    if(!(cond))                                                          \
    {                                                                    \
        LogError("\n                          \n"                        \
                 "****************************\n"                        \
                 "*                          *\n"                        \
                 "*    !ASSERTION FAILED!    *\n"                        \
                 "*                          *\n"                        \
                 "****************************");                        \
        LogError("CONDITION: "#cond"\n"                                  \
                 "MESSAGE:   " msg "\n"                                  \
                 "FILE: " __FILE__ ":" _UTIX_MACRO_STR_(__LINE__)"\n");  \
        _UTIX_DEBUG_BREAK_();                                            \
    }                                                                    \
}





#endif
#endif
