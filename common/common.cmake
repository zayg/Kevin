# kevin common

file (GLOB KEVIN_COMMON_SRCS "${PROJECT_SOURCE_DIR}/common/*.cpp")
file (GLOB KEVIN_LOGGING_SRCS "${PROJECT_SOURCE_DIR}/common/klogging/*.cpp")
file (GLOB KEVIN_TEST_SRCS "${PROJECT_SOURCE_DIR}/common/ktest/*.cpp")

add_library (kevin-test OBJECT ${KEVIN_TEST_SRCS})
add_library (kevin-logging OBJECT ${KEVIN_LOGGING_SRCS})
add_library (kevin-common OBJECT ${KEVIN_COMMON_SRCS})
