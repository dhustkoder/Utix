/*

XLIB - utility library from XChip
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

#include <cerrno>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <Xlib/Log.h>







namespace xlib {


static std::string errstr(512, '\0');

void Log(const char* fmtString, ...) noexcept
{
	va_list args;
	va_start(args, fmtString);

	if(std::vfprintf(stdout, fmtString, args) < 0 )
		LogError("Failed to print Log");

	std::fprintf(stdout, "\n");
	va_end(args);
}


void LogError(const char* fmtString, ...) noexcept
{
	const auto errnoCode = errno;
	va_list args;
	va_start(args, fmtString);
	
	// write the message to buffer and get writeSize
	const auto errstrSize = errstr.size();
	const auto writeSize = std::vsnprintf(&errstr[0], errstrSize, fmtString, args);

	if(writeSize > 0)
	{
		if(errnoCode) { 
			std::snprintf(&errstr[writeSize], errstrSize - writeSize, ": %s", strerror(errnoCode));
			errno = 0;
		}
		else { 
			errstr[writeSize] = '\0';
		}
		// print
		std::fprintf(stderr, "%s\n", errstr.c_str());
	}
	else
	{
		perror("Error in LogError vsprintf!!:");
	}


	va_end(args);
}

const std::string& GetLastLogError() noexcept
{
	return errstr;
}







}
