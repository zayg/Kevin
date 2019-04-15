# kevin thirdparty

set (THIRDPARTY_CMAKE "${PROJECT_BINARY_DIR}/thirdparty")

set (protobuf_BUILD_TESTS OFF CACHE BOOL "")
add_subdirectory ("${PROJECT_SOURCE_DIR}/thirdparty/protobuf/cmake" "${THIRDPARTY_CMAKE}/protobuf")

set (LEVELDB_BUILD_TESTS OFF CACHE BOOL "")
set (LEVELDB_BUILD_BENCHMARKS OFF CACHE BOOL "")
add_subdirectory ("${PROJECT_SOURCE_DIR}/thirdparty/leveldb" "${THIRDPARTY_CMAKE}/leveldb")

set (JSONCPP_WITH_TESTS OFF CACHE BOOL "")
set (JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "")
add_subdirectory ("${PROJECT_SOURCE_DIR}/thirdparty/jsoncpp" "${THIRDPARTY_CMAKE}/jsoncpp")

set (SPDLOG_BUILD_TESTS OFF CACHE BOOL "")
set (SPDLOG_INSTALL OFF CACHE BOOL "")
add_subdirectory ("${PROJECT_SOURCE_DIR}/thirdparty/spdlog" "${THIRDPARTY_CMAKE}/spdlog")

set (KEVIN_THIRDPARTY_INCLUDES
    "${PROJECT_SOURCE_DIR}/thirdparty/protobuf/src/"
    "${PROJECT_SOURCE_DIR}/thirdparty/leveldb/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/jsoncpp/include"
    "${PROJECT_SOURCE_DIR}/thirdparty/spdlog/include"
    )
set (KEVIN_THIRDPARTY_PROTOC "${PROJECT_BINARY_DIR}/thirdparty/protobuf/protoc")

add_custom_target (kevin-protoc DEPENDS protoc)
# TODO(yihao) try to make kevin-thirdparties a true target
set (kevin-thirdparties libprotobuf leveldb spdlog)

