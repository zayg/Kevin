#pragma once

namespace kevin {
namespace raft {

enum class RaftStateType {
    RAFT_STATE_LEARNER,
    RAFT_STATE_FOLLOWER,
    RAFT_STATE_CANDIDATE,
    RAFT_STATE_LEADER,
};

enum class RaftError : int {
    RAFT_OK = 0,
    RAFT_NOT_LEADER,

    // Unknown error code. Shouldn't use it.
    RAFT_UNKNOWN
};

} // namespace kevin
} // namespace raft
