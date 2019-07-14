#pragma once

#include "raft/raft-common.h"
#include <cstdint>

namespace kevin {
namespace raft {


struct Log;
class VoteRequest;
class VoteResponse;
class AppendLogRequest;
class AppendLogResponse;
class RaftGroup;


class RaftEventProcessor {
public:
    RaftEventProcessor(RaftStateType type) : m_type(type) {}

    RaftStateType m_type;

protected:
    // event handlers
    virtual kevin::raft::RaftError _handleUserLog(const Log &log) = 0;

    virtual kevin::raft::RaftError _handleVoteRequest(
            const VoteRequest &req,
            RaftStateType *stateChangeTo) = 0;

    virtual kevin::raft::RaftError _handleVoteResponse(const VoteResponse &resp) = 0;
    virtual kevin::raft::RaftError _handleAppendLogRequest(const AppendLogRequest &req) = 0;
    virtual kevin::raft::RaftError _handleAppendLogResponse(const AppendLogResponse &resp) = 0;
    virtual kevin::raft::RaftError _handleElectionTimerExpired() = 0;
    virtual kevin::raft::RaftError _handleNewTerm(int64_t term) = 0;

    friend class RaftGroup;
};

} // namespace kevin
} // namespace raft 

