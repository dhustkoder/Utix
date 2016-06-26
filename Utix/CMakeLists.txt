
set(UTIX_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(UTIX_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Utix")
set(UTIX_TEST_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Test")

#include Xlib
include_directories(${UTIX_INCLUDE_DIR})
# builds the library
add_subdirectory(${UTIX_SRC_DIR})


if( BUILD_UTIX_TEST )
	add_subdirectory(${UTIX_TEST_SRC_DIR})
endif()
