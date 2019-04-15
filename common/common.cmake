# kevin common

file (GLOB KEVIN_COMMON_SRCS "${PROJECT_SOURCE_DIR}/common/*.cpp")
file (GLOB KEVIN_LOGGING_SRCS "${PROJECT_SOURCE_DIR}/common/logging/*.cpp")
file (GLOB KEVIN_TEST_SRCS "${PROJECT_SOURCE_DIR}/common/test/*.cpp")

add_library (kevin-test OBJECT ${KEVIN_TEST_SRCS})
# TODO(yihao) Implement logging library in the future.
# add_library (kevin-logging OBJECT ${KEVIN_LOGGING_SRCS})
add_library (kevin-common OBJECT ${KEVIN_COMMON_SRCS})

# unittest for kevin-test
add_executable (kevin-test-ut
    "${PROJECT_SOURCE_DIR}/common/test/unittests/test_basics.cpp"
    $<TARGET_OBJECTS:kevin-test>)
