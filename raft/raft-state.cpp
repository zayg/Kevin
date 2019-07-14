#include "raft/raft-group.h"
#include "raft/raft-log.h"
#include "raft/raft-state.h"
#include "raft/raft-store.h"

#include "proto/raft.pb.h"

using namespace kevin::raft;


// TODO(yihao)
// 1. We have only one Raft event queue. That will need a dedicated
// thread outside the state machine. The implementation should
// make sure the event handling to be as non-blocking as possible.
// Also queue system is infamous for latency, think about split the
// queue or have multiple event consumers.
// 2. One goal is to implement the Raft state machine in a way
// transparenting to the RPC frame. The service layer should be able to 
// register a callback into the processors for responsing a RPC. Out-
// side the raft state machine, we can have a set of IO threads
// for responsing/receiving the RPCs.
// 3. State transfer may need extra steps. E.g., tranferring to
// RAFT_FOLLOWER requires to serialize the new term and new votee.
// In this step, the IO seems can be asynchronous. We can have a
// set of disk IO threads doing this outside the Raft state machine.
// 4. The Expiration timers should have term inside. If the term
// is out-dated. It shouldn't take any effect. The timer can
// share the disk IO threads for ticking.


#define TERM_CHECK(request, stateChangeTo) \
    if (request.term() < m_raftGroup->m_metaStore->term_) { \
        stateChangeTo = RaftStateType::RAFT_STATE_FOLLOWER; \
        return RaftError::RAFT_STATE_TRANSFER; \
    } \


RaftError
RaftFollower::_handleUserLog(const Log &log)
{
    return RaftError::RAFT_NOT_LEADER;
}


RaftError
RaftFollower::_handleVoteRequest(
        const VoteRequest &req,
        RaftStateType *stateChangeTo)
{
    *stateChangeTo = this->m_type;
    TERM_CHECK(req, *stateChangeTo);
    return RaftError::RAFT_OK;
}


RaftError
RaftFollower::_handleVoteResponse(const VoteResponse &resp)
{
    RaftError err;
    return err;
}


RaftError
RaftFollower::_handleAppendLogRequest(const AppendLogRequest &req)
{
    RaftError err;
    return err;
}


RaftError
RaftFollower::_handleAppendLogResponse(const AppendLogResponse &resp)
{
    RaftError err;
    return err;
}


RaftError
RaftFollower::_handleElectionTimerExpired()
{
    RaftError err;
    return err;
}


RaftError
RaftFollower::_handleNewTerm(int64_t term)
{
    RaftError err;
    return err;
}




RaftError
RaftCandidate::_handleUserLog(const Log &log)
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleVoteRequest(
        const VoteRequest &req,
        RaftStateType *stateChangeTo)
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleVoteResponse(const VoteResponse &resp)
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleAppendLogRequest(const AppendLogRequest &req)
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleAppendLogResponse(const AppendLogResponse &resp)
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleElectionTimerExpired()
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleNewTerm(int64_t term)
{
    RaftError err;
    return err;
}




RaftError
RaftLeader::_handleUserLog(const Log &log)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleVoteRequest(
        const VoteRequest &req,
        RaftStateType *stateChangeTo)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleVoteResponse(const VoteResponse &resp)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleAppendLogRequest(const AppendLogRequest &req)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleAppendLogResponse(const AppendLogResponse &resp)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleElectionTimerExpired()
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleNewTerm(int64_t term)
{
    RaftError err;
    return err;
}


