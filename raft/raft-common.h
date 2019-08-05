#pragma once

#include <cstdint>

namespace kevin {
namespace raft {

#define unlikely(x) __builtin_expect(!!(x), 0) 
#define likely(x) __builtin_expect(!!(x), 1)

enum class RaftStateType : int32_t {
    RAFT_STATE_LEARNER,
    RAFT_STATE_FOLLOWER,
    RAFT_STATE_CANDIDATE,
    RAFT_STATE_LEADER,
};

enum class RaftError : int32_t {
    RAFT_OK = 0,
    RAFT_NOT_LEADER,
    RAFT_NOT_FOLLOWER,
    RAFT_NOT_CANDIDATE,
    RAFT_HIGHER_TERM,
    RAFT_LOWER_TERM,
    RAFT_LOG_MISSING,
    // Unknown error code. Shouldn't use it.
    RAFT_UNKNOWN
};

} // namespace kevin
} // namespace raft
