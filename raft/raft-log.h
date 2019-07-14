#pragma once

namespace kevin {
namespace raft {

enum class RaftLogType : int {
    INSERT = 0,
    DELETE = 1,

    // update replicaIds of the new cluster
    QUORUM = 2,
};

struct Log {
    inline Log(int32_t len, RaftLogType type)
        : keyLen(len) {
        *reinterpret_cast<RaftLogType*>(payload) = type; 
    }

    int32_t keyLen;
    // the first sizeof(OpType) bytes is the OpType
    char payload[sizeof(RaftLogType)];
    // the rest of bytes is the key/value pairs
};

#define KEY_PTR(log) (log->payload + sizeof(RaftLogType))
#define VALUE_PTR(log) (log->payload + sizeof(RaftLogType) + log->keyLen)

} // namespace kevin
} // namespace raft 
