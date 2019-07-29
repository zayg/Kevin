#pragma once

#include <cstdint>

namespace kevin {
namespace raft {

enum class RaftStateType : int32_t {
    RAFT_STATE_LEARNER,
    RAFT_STATE_FOLLOWER,
    RAFT_STATE_CANDIDATE,
    RAFT_STATE_LEADER,
};

enum class RaftError : int32_t {
    RAFT_OK = 0,
    RAFT_NOT_LEADER,
    RAFT_INVALID_TERM,
    // Unknown error code. Shouldn't use it.
    RAFT_UNKNOWN
};

} // namespace kevin
} // namespace raft
