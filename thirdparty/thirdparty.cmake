set (THIRDPARTY_CMAKE "${PROJECT_BINARY_DIR}/thirdparty")

message (STATUS "Building ProtocolBuffer...")
set (protobuf_BUILD_TESTS OFF CACHE BOOL "Close protobuf_BUILD_TESTS to avoid the test compilation.")
add_subdirectory ("${PROJECT_SOURCE_DIR}/thirdparty/protobuf/cmake" "${THIRDPARTY_CMAKE}/protobuf")

message (STATUS "Building LevelDb...")
set (LEVELDB_BUILD_TESTS OFF CACHE BOOL "Close LEVELDB_BUILD_TESTS to avoid the test compilation.")
set (LEVELDB_BUILD_BENCHMARKS OFF CACHE BOOL "Close LEVELDB_BUILD_BENCHMARKS to avoid the benchmark compilation.")
add_subdirectory ("${PROJECT_SOURCE_DIR}/thirdparty/leveldb" "${THIRDPARTY_CMAKE}/leveldb")

set (KEVIN_THIRDPARTY_LIBS libprotobuf leveldb)
set (KEVIN_THIRDPARTY_INCLUDES
    "${PROJECT_SOURCE_DIR}/thirdparty/protobuf/src/"
    "${PROJECT_SOURCE_DIR}/thirdparty/leveldb/include")
set (KEVIN_THIRDPARTY_PROTOC "${PROJECT_BINARY_DIR}/thirdparty/protobuf/protoc")

add_custom_target (kevin-thirdparty DEPENDS libprotoc)

