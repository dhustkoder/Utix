project(UTIX_TEST)

# exceptions and rtti are NOT used on Utility.
# static version
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")


file(GLOB_RECURSE SRC ./*.cpp)
file(GLOB_RECURSE HEADERS ${UTIX_INCLUDE_DIR}/Utix/*.h)
add_executable(${PROJECT_NAME} ${HEADERS} ${SRC})
target_link_libraries(${PROJECT_NAME} Utix benchmark)
message( "FLAGS FOR TEST : " ${CMAKE_CXX_FLAGS} )

INSTALL(TARGETS ${PROJECT_NAME}  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Test/)
