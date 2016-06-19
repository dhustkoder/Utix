project(Utix)

# exceptions and rtti are NOT used on Utility.
# static version
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-exceptions -fno-rtti")


file(GLOB_RECURSE SRC ./*.cpp)
file(GLOB_RECURSE HEADERS XChip/*.h)
add_library(${PROJECT_NAME} ${HEADERS} ${SRC})
target_link_libraries(${PROJECT_NAME} -ldl)



INSTALL(TARGETS ${PROJECT_NAME}  DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib/)


# fpic version
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

add_library(UtixFPIC ${HEADERS} ${SRC})
target_link_libraries(UtixFPIC -ldl)

INSTALL(TARGETS UtixFPIC DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib/)
