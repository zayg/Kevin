#include "raft/raft-group.h"
#include "raft/raft-log.h"
#include "raft/raft-state.h"
#include "raft/raft-store.h"

#include "proto/raft.pb.h"

#include <google/protobuf/util/message_differencer.h>

using namespace kevin::raft;
using namespace google::protobuf::util;


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


#define REJECT_LOWER_TERM(request, response) \
    if ((request).term() < m_raftGroup->m_metaStore->term_) { \
        (response).set_error_code(int32_t(RaftError::RAFT_INVALID_TERM)); \
        (response).set_term(m_raftGroup->m_metaStore->term_); \
        cb(&(response)); \
        return static_cast<RaftError>((response).error_code()); \
    } \

#define CHECK_FOLLOWER_TRANSFER(message, stateChangeTo) \
    if ((message).term() > m_raftGroup->m_metaStore->term_) { \
        m_raftGroup->m_metaStore->term_ = (message).term(); \
        stateChangeTo = RaftStateType::RAFT_STATE_FOLLOWER; \
        return RaftError::RAFT_OK; \
    } \


RaftError
RaftFollower::_handleUserLog(const Log &log)
{
    return RaftError::RAFT_NOT_LEADER;
}


RaftError
RaftFollower::_handleVoteRequest(
        const VoteRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(VoteResponse *)> &&cb)
{
    // Initialized with errc = 0, term = 0, vote = 0.
    VoteResponse *resp = new VoteResponse();

    // Initialize with its original state.
    *stateChangeTo = this->m_type;

    REJECT_LOWER_TERM(req, *resp);
    
    // It is safe to update the term here in memory only.
    // The reason is it doestn't violate the 3 invariants:
    // 1. If there is a new votee, we will persist it in the end of this function.
    //    As long as new vote is persistent, election safety is maintained.
    // 2. Log matching is still right, since the persistent (term, lsn) in the log
    //    still can be generated once and the AppendEntries logic doesn't change.
    // 3. Leader completeness is maintained, since if we persisted a (term, lsn),
    //    then in recovery we can recover the term (no need to persist here).
    //
    // TODO(yihao) Anyway need a provement.
    if (m_raftGroup->m_metaStore->term_ < req.term()) {
        m_raftGroup->m_metaStore->term_ = req.term();
    }

    auto *metaStore = m_raftGroup->m_metaStore;
    bool noVoteYet = !metaStore->hasPersistedVotee(req.term());
    if ((noVoteYet
            || MessageDifferencer::Equivalent(metaStore->votee_, req.candidate()))
        // If the candidate's lsn is at least as new as the follower
        && (metaStore->latestTerm_ < req.latest_term()
            || metaStore->latestLsn_ <= req.latest_lsn())) {

        resp->set_vote(1);
        // TODO(yihao) Can we change to follower before persist the new votee?
        *stateChangeTo = RaftStateType::RAFT_STATE_FOLLOWER;
        if (noVoteYet) {
            return metaStore->persistVotee(
                    req.term(), req.candidate(), std::bind(cb, resp));
        }
    }

    resp->set_term(metaStore->term_);
    cb(resp);
    return RaftError::RAFT_OK;
}


RaftError
RaftFollower::_handleVoteResponse(
        const VoteResponse &resp,
        RaftStateType *stateChangeTo)
{
    CHECK_FOLLOWER_TRANSFER(resp, *stateChangeTo);
    return RaftError::RAFT_OK;
}


RaftError
RaftFollower::_handleAppendLogRequest(
        const AppendLogRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(AppendLogResponse *)> &&cb)
{
    RaftError err;
    return err;
}


RaftError
RaftFollower::_handleAppendLogResponse(
        const AppendLogResponse &resp,
        RaftStateType *stateChangeTo)
{
    CHECK_FOLLOWER_TRANSFER(resp, *stateChangeTo);
    return RaftError::RAFT_OK;
}


RaftError
RaftFollower::_handleElectionTimerExpired()
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
        RaftStateType *stateChangeTo,
        std::function<void(VoteResponse *)> &&cb)
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleVoteResponse(
        const VoteResponse &resp,
        RaftStateType *stateChangeTo)
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleAppendLogRequest(
        const AppendLogRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(AppendLogResponse *)> &&cb)
{
    RaftError err;
    return err;
}


RaftError
RaftCandidate::_handleAppendLogResponse(
        const AppendLogResponse &resp,
        RaftStateType *stateChangeTo)
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
RaftLeader::_handleUserLog(const Log &log)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleVoteRequest(
        const VoteRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(VoteResponse *)> &&cb)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleVoteResponse(
        const VoteResponse &resp,
        RaftStateType *stateChangeTo)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleAppendLogRequest(
        const AppendLogRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(AppendLogResponse *)> &&cb)
{
    RaftError err;
    return err;
}


RaftError
RaftLeader::_handleAppendLogResponse(
        const AppendLogResponse &resp,
        RaftStateType *stateChangeTo)
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


