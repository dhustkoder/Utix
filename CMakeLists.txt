cmake_minimum_required(VERSION 2.8.8)
project(Utix)

option(BUILD_UTIX_TEST OFF)
option(BUILD_UTIX_FPIC OFF)
option(ADDRESS_SANITIZER OFF)
option(MEMORY_SANITIZER OFF)
option(ENABLE_LTO OFF)

# compiler settings flags
set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++11 -pedantic -pedantic-errors")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -DNDEBUG -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-unwind-tables -g0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g3 -D_DEBUG -fno-omit-frame-pointer")


if(NOT CMAKE_BUILD_TYPE)
	message(STATUS "No build type selected! default to Release")
	set(CMAKE_BUILD_TYPE "Release")
endif()


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









#directories
set(UTIX_INCLUDE_DIR "./Utix/include")
set(UTIX_SRC_DIR "./Utix/src/Utix")
set(UTIX_TEST_SRC_DIR "./Utix/src/Test")

#files 
file(GLOB_RECURSE UTIX_SRC ${UTIX_SRC_DIR}/*.cpp)
file(GLOB_RECURSE UTIX_TEST_SRC ${UTIX_TEST_SRC_DIR}/*.cpp)
file(GLOB_RECURSE UTIX_HEADERS ${UTIX_INCLUDE_DIR}/*.h)

# include dir
include_directories(${UTIX_INCLUDE_DIR})

# exceptions and rtti are NOT used on Utility.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")


# compile normal static lib version 
add_library(${PROJECT_NAME} ${UTIX_HEADERS} ${UTIX_SRC})
target_link_libraries(${PROJECT_NAME} -ldl)
INSTALL(TARGETS ${PROJECT_NAME}  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib/)


# compile  fpic version
if(BUILD_UTIX_FPIC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	add_library(UtixFPIC ${UTIX_HEADERS} ${UTIX_SRC})
	target_link_libraries(UtixFPIC -ldl)
	INSTALL(TARGETS UtixFPIC DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib/)
endif()


# build test 
if( BUILD_UTIX_TEST )
	message( "FLAGS FOR TEST : " ${CMAKE_CXX_FLAGS} )
	add_executable(UTIX_TEST ${UTIX_HEADERS} ${UTIX_TEST_SRC})
	target_link_libraries(UTIX_TEST Utix)
	INSTALL(TARGETS UTIX_TEST  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Test/)
endif()



