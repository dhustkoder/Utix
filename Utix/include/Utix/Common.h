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




#ifndef _UTIX_COMMON_H_
#define _UTIX_COMMON_H_
#include <string>
#include "BaseTraits.h"
#include "Timer.h"
#include "Log.h"

namespace utix {

template<class T>
inline enable_if_t<is_numeric<T>::value> Clamp(const T min, const T max, T* const lval)
{
	if(*lval > max)
		*lval = max;
	else if(*lval < min)
		*lval = min;
}


template<class T>
inline enable_if_t<!is_numeric<T>::value> Clamp(const T& min, const T& max, T* const lval)
{
	if(*lval > max)
		*lval = max;
	else if(*lval < min)
		*lval = min;
}





inline void Sleep(const Nano& nano)
{
	/* high precision sleep unix */
#if defined(__linux__) || defined(__CYGWIN32__) || defined(__APPLE__)
	static timespec _sleep{ 0, 0 };
	_sleep.tv_nsec = nano.count();
	if(nanosleep(&_sleep, NULL)==-1)
          LogError("nanosleep error");
     
#elif _WIN32 
	using namespace std::chrono;
	::Sleep(static_cast<DWORD>(duration_cast<Milli>(nano).count()));
#endif
}



std::string GetFullProcName();
std::string GetFullProcDir();














}



































#endif
