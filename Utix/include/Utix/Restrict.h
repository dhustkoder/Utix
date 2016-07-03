#ifndef UTIX_RESTRICT_H_
#define UTIX_RESTRICT_H_
// original source: https://github.com/dolphin-emu/dolphin/blob/3ff56aa192f952cb6ad2e9c662d4a368efd21e6f/Externals/libiconv-1.14/config.h

#define restrict __restrict
/* Work around a bug in Sun C++: it does not support _Restrict or
   __restrict__, even though the corresponding Sun C compiler ends up with
   "#define restrict _Restrict" or "#define restrict __restrict__" in the
   previous line.  Perhaps some future version of Sun C++ will work with
   restrict; if so, hopefully it defines __RESTRICT like Sun C does.  */
#if defined __SUNPRO_CC && !defined __RESTRICT
# define _Restrict
# define __restrict__
#endif






#endif
