#pragma once

#include "raft/raft-common.h"
#include "raft/raft-event-processor.h"
#include <cstdint>

namespace kevin {
namespace raft {


struct Log;
class VoteRequest;
class VoteResponse;
class AppendLogRequest;
class AppendLogResponse;
class RaftGroup;


class RaftFollower : public RaftEventProcessor {
public:
    RaftFollower(RaftGroup *raftGroup)
        : RaftEventProcessor(RaftStateType::RAFT_STATE_FOLLOWER),
          m_raftGroup(raftGroup) {}

protected:
    kevin::raft::RaftError _handleUserLog(const Log &log) final;

    kevin::raft::RaftError _handleVoteRequest(
            const VoteRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(VoteResponse *)> &&cb) final;

    kevin::raft::RaftError _handleVoteResponse(
            const VoteResponse &resp,
            RaftStateType *stateChangeTo) final;

    kevin::raft::RaftError _handleAppendLogRequest(
            const AppendLogRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(AppendLogResponse *)> &&cb) final;

    kevin::raft::RaftError _handleAppendLogResponse(
            const AppendLogResponse &resp,
            RaftStateType *stateChangeTo) final;

    kevin::raft::RaftError _handleElectionTimerExpired(
            int64_t term,
            RaftStateType *stateChangeTo) final;

private:
    RaftGroup *m_raftGroup;
};


class RaftCandidate : public RaftEventProcessor {
public:
    RaftCandidate(RaftGroup *raftGroup)
        : RaftEventProcessor(RaftStateType::RAFT_STATE_CANDIDATE),
          m_raftGroup(raftGroup) {}

protected:
    kevin::raft::RaftError _handleUserLog(const Log &log) final;

    kevin::raft::RaftError _handleVoteRequest(
            const VoteRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(VoteResponse *)> &&cb) final;

    kevin::raft::RaftError _handleVoteResponse(
            const VoteResponse &resp,
            RaftStateType *stateChangeTo) final;

    kevin::raft::RaftError _handleAppendLogRequest(
            const AppendLogRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(AppendLogResponse *)> &&cb) final;

    kevin::raft::RaftError _handleAppendLogResponse(
            const AppendLogResponse &resp,
            RaftStateType *stateChangeTo) final;

    kevin::raft::RaftError _handleElectionTimerExpired(
            int64_t term,
            RaftStateType *stateChangeTo) final;

private:
    RaftGroup *m_raftGroup;
};


class RaftLeader : public RaftEventProcessor {
public:
    RaftLeader(RaftGroup *raftGroup)
        : RaftEventProcessor(RaftStateType::RAFT_STATE_LEADER),
          m_raftGroup(raftGroup) {}

protected:
    kevin::raft::RaftError _handleUserLog(const Log &log) final;

    kevin::raft::RaftError _handleVoteRequest(
            const VoteRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(VoteResponse *)> &&cb) final;

    kevin::raft::RaftError _handleVoteResponse(
            const VoteResponse &resp,
            RaftStateType *stateChangeTo) final;

    kevin::raft::RaftError _handleAppendLogRequest(
            const AppendLogRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(AppendLogResponse *)> &&cb) final;

    kevin::raft::RaftError _handleAppendLogResponse(
            const AppendLogResponse &resp,
            RaftStateType *stateChangeTo) final;

    kevin::raft::RaftError _handleElectionTimerExpired(
            int64_t term,
            RaftStateType *stateChangeTo) final;

private:
    RaftGroup *m_raftGroup;
};

} // namespace kevin
} // namespace raft 

