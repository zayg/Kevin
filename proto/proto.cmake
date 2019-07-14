# kevin proto

set (KEVIN_PROTO_PATH ${PROJECT_SOURCE_DIR}/proto)
set (KEVIN_PROTO_SRCS ${KEVIN_PROTO_PATH}/raft.pb.cc
                      ${KEVIN_PROTO_PATH}/raft-service.pb.cc)

add_custom_command (
    OUTPUT ${KEVIN_PROTO_PATH}/raft.pb.h
           ${KEVIN_PROTO_PATH}/raft.pb.cc
    COMMAND ${KEVIN_THIRDPARTY_PROTOC} ${KEVIN_PROTO_PATH}/raft.proto
            -I ${KEVIN_PROTO_PATH} --cpp_out=${KEVIN_PROTO_PATH}
    DEPENDS kevin-protoc
    )

add_custom_command (
    OUTPUT ${KEVIN_PROTO_PATH}/raft-service.pb.h
           ${KEVIN_PROTO_PATH}/raft-service.pb.cc
    COMMAND ${KEVIN_THIRDPARTY_PROTOC}
            ${KEVIN_PROTO_PATH}/raft-service.proto
            -I ${KEVIN_PROTO_PATH} --cpp_out=${KEVIN_PROTO_PATH}
    DEPENDS kevin-protoc
    )

add_library (kevin-proto OBJECT ${KEVIN_PROTO_SRCS})

