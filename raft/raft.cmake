# kevin raft 

file (GLOB KEVIN_RAFT_SRCS "${PROJECT_SOURCE_DIR}/raft/*.cpp")

add_library (kevin-raft OBJECT ${KEVIN_RAFT_SRCS})

