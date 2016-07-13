cmake_minimum_required(VERSION 2.8.8)
project(Utix)

option(ADDRESS_SANITIZER OFF)
option(MEMORY_SANITIZER OFF)
option(ENABLE_LTO OFF)

# compiler settings flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -std=c++11 -pedantic -pedantic-errors")

if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected! default to release")
	set(CMAKE_BUILD_TYPE "Release")
endif()



# "Release" full optimization , no debug info.
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections -g0")



# "Debug" full debug information, no optimization, asserts enabled
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g3 -D_DEBUG -fno-omit-frame-pointer")


# "Bench" better code generation but keep debug information
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Bench")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2  -DNDEBUG -fno-omit-frame-pointer")
endif()


if( ADDRESS_SANITIZER )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
endif()

if( UNDEFINED_SANITIZER )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined")
endif()


if( ENABLE_LTO )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
endif()




add_subdirectory(Utix)
