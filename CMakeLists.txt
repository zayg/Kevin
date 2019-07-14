cmake_minimum_required (VERSION 3.0)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11")
endif()

set (KEVIN_VERSION_MAJOR 0)
set (KEVIN_VERSION_MINOR 1)

project (Kevin VERSION "${KEVIN_VERSION_MAJOR}.${KEVIN_VERSION_MINOR}"
               LANGUAGES "CXX")

set (PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")

# options
option (KEVIN_DEBUG "KEVIN_DEBUG" ON)
option (KEVIN_REGRET "KEVIN_REGRET" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    "${PROJECT_SOURCE_DIR}/kevin-config.h.in"
    "${PROJECT_BINARY_DIR}/kevin-config.h"
    )
       
include ("${PROJECT_SOURCE_DIR}/thirdparty/thirdparty.cmake")
include_directories (
    # so that we will find kevin-config.h
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_BINARY_DIR}"
    "${KEVIN_THIRDPARTY_INCLUDES}")
include ("${PROJECT_SOURCE_DIR}/proto/proto.cmake")
include ("${PROJECT_SOURCE_DIR}/common/common.cmake")
include ("${PROJECT_SOURCE_DIR}/raft/raft.cmake")

add_subdirectory ("${PROJECT_SOURCE_DIR}/server")
add_subdirectory ("${PROJECT_SOURCE_DIR}/client")


