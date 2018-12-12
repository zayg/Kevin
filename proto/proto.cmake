# kevin proto

set (KEVIN_PROTO_PATH ${PROJECT_SOURCE_DIR}/proto)
set (KEVIN_PROTO_SRCS ${KEVIN_PROTO_PATH}/consensus.pb.cc ${KEVIN_PROTO_PATH}/consensus-service.pb.cc)

add_custom_command (
    OUTPUT ${KEVIN_PROTO_PATH}/consensus.pb.h ${KEVIN_PROTO_PATH}/consensus.pb.cc
    COMMAND ${KEVIN_THIRDPARTY_PROTOC} ${KEVIN_PROTO_PATH}/consensus.proto -I ${KEVIN_PROTO_PATH} --cpp_out=${KEVIN_PROTO_PATH}
    DEPENDS kevin-thirdparty 
    )

add_custom_command (
    OUTPUT ${KEVIN_PROTO_PATH}/consensus-service.pb.h ${KEVIN_PROTO_PATH}/consensus-service.pb.cc
    COMMAND ${KEVIN_THIRDPARTY_PROTOC} ${KEVIN_PROTO_PATH}/consensus-service.proto -I ${KEVIN_PROTO_PATH} --cpp_out=${KEVIN_PROTO_PATH}
    DEPENDS kevin-thirdparty
    )

add_custom_target (kevin-proto DEPENDS ${KEVIN_PROTO_SRCS})
