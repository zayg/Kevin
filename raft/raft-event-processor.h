#pragma once

#include "raft/raft-common.h"
#include <cstdint>
#include <functional>

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
    // Event Handlers
    // Implemetation Contracts:
    // 1. All returned error code must be logical and determined
    //    (No system related code, like RPC/Device errors). System error code
    //    can be set in the responses.
    // 2. Add handlers should be non-blocking. Put blocking logics 
    //    in the callback.
    virtual kevin::raft::RaftError _handleUserLog(const Log &log) = 0;

    virtual kevin::raft::RaftError _handleVoteRequest(
            const VoteRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(VoteResponse *)> &&cb) = 0;

    virtual kevin::raft::RaftError _handleVoteResponse(
            const VoteResponse &resp,
            RaftStateType *stateChangeTo) = 0;

    virtual kevin::raft::RaftError _handleAppendLogRequest(
            const AppendLogRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(AppendLogResponse *)> &&cb) = 0;

    virtual kevin::raft::RaftError _handleAppendLogResponse(
            const AppendLogResponse &resp,
            RaftStateType *stateChangeTo) = 0;

    virtual kevin::raft::RaftError _handleElectionTimerExpired() = 0;

    friend class RaftGroup;
};

} // namespace kevin
} // namespace raft 

