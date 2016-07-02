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

#ifndef UTIX_LOG_H_
#define UTIX_LOG_H_


#include <string>
#include "Ints.h"

namespace utix {



namespace literals 
{
	inline std::string operator"" _s(const char* str, size_t) { return std::string(str); }
}


extern void Log(const char* fmtString, ...) noexcept;
extern void LogError(const char* fmtString, ...) noexcept;
extern const std::string& GetLastLogError() noexcept;


















}



#endif // UTIX_LOG_H_
