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

#ifndef UTIX_ASSERT_H_
#define UTIX_ASSERT_H_


#ifndef _DEBUG
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
#define UTIX_DEBUG_BREAK_() __debugbreak()
#elif defined(__linux__) || defined(__APPLE__)
#define UTIX_DEBUG_BREAK_() raise(SIGTRAP)
#else 
#error Utix Assert - Unknown Platafor
#endif

#define UTIX_MACRO_STR_EX_(x) #x
#define UTIX_MACRO_STR_(x) UTIX_MACRO_STR_EX_(x)

#define ASSERT_MSG(cond, msg)                                            \
{                                                                        \
                                                                         \
    if(!(cond))                                                          \
    {                                                                    \
        utix::LogError("\n                          \n"                  \
                 "****************************\n"                        \
                 "*                          *\n"                        \
                 "*    !ASSERTION FAILED!    *\n"                        \
                 "*                          *\n"                        \
                 "****************************");                        \
        utix::LogError("CONDITION: "#cond"\n"                            \
                 "MESSAGE:   " msg "\n"                                  \
                 "FILE: " __FILE__ ":" UTIX_MACRO_STR_(__LINE__)"\n");   \
        UTIX_DEBUG_BREAK_();                                             \
    }                                                                    \
}



#endif



#endif // UTIX_ASSERT_H_
