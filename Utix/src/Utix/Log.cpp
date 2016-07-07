/*

utix - utility library from XChip
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


#include <errno.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif

#include <Utix/Log.h>







namespace utix {


static std::string errstr(512, '\0');


void Log(const char* fmtString, ...) noexcept
{
	va_list args;
	va_start(args, fmtString);
	
	#ifdef __ANDROID__
	__android_log_vprint(ANDROID_LOG_INFO, "LOG_INFO", fmtString, args);
	#else
	if(vfprintf(stdout, fmtString, args) < 0 )
		LogError("Failed to print Log");
	fprintf(stdout, "\n");
	#endif
	va_end(args);
}


void LogError(const char* fmtString, ...) noexcept
{
	const auto errnoCode = errno;
	va_list args;
	va_start(args, fmtString);

	// write the message to buffer and get writeSize
	const auto errstrSize = errstr.size();
	const auto writeSize = vsnprintf(&errstr[0], errstrSize, fmtString, args);
	
	va_end(args);

	if(writeSize > 0)
	{
		if(errnoCode) { 
			snprintf(&errstr[writeSize], errstrSize - writeSize, ": %s", strerror(errnoCode));
			errno = 0;
		}
		else { 
			errstr[writeSize] = '\0';
		}
		
		// print to stderr
		#ifdef __ANDROID__
		__android_log_print(ANDROID_LOG_ERROR, "LOG_ERROR", "%s", errstr.c_str());
		#else
		fprintf(stderr, "%s\n", errstr.c_str());
		#endif
	}
	else
	{
		perror("Error in LogError vsprintf!!:");
	}

}


const std::string& GetLastLogError() noexcept
{
	return errstr;
}







}
