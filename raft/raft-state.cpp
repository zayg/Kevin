#include "raft/raft-group.h"
#include "raft/raft-log.h"
#include "raft/raft-state.h"
#include "raft/raft-store.h"
#include "common/error.h"
#include "proto/raft.pb.h"

#include <google/protobuf/util/message_differencer.h>

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


#define REJECT_LOWER_TERM(metaStore, request, response) \
    if ((request).term() < metaStore->term_) { \
        (response).set_error_code(int32_t(RaftError::RAFT_LOWER_TERM)); \
        (response).set_term(metaStore->term_); \
        cb(&(response)); \
        return RaftError::RAFT_LOWER_TERM; \
    } \

#define CHANGE_TO_FOLLOWER_IF_TERM_HIGHER(message, stateChangeTo, ret) \
    if ((message).term() > m_raftGroup->m_metaStore->term_) { \
        m_raftGroup->m_metaStore->term_ = (message).term(); \
        stateChangeTo = RaftStateType::RAFT_STATE_FOLLOWER; \
        ret = RaftError::RAFT_HIGHER_TERM; \
    } \


namespace kevin {
namespace raft {

RaftError
_handleVoteRequest(
        const VoteRequest &req,
        RaftMetaStore *metaStore,
        RaftStateType *stateChangeTo,
        std::function<void(VoteResponse *)> &&cb)
{
    // Initialized with errc = 0, term = 0, vote = 0.
    auto *resp = new VoteResponse();

    REJECT_LOWER_TERM(metaStore, req, *resp);
    
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
    metaStore->updateTerm(req.term());

    bool noVoteYet = !metaStore->hasPersistedVotee(req.term());
    if ((noVoteYet
            || MessageDifferencer::Equivalent(metaStore->votee_, req.candidate()))
        // If the candidate's lsn is at least as new as here
        && metaStore->staleThan(req.latest_term(), req.latest_lsn())) {

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
RaftFollower::_handleUserLog(const Log &log)
{
    // TODO(yihao) return votee as hint
    return RaftError::RAFT_NOT_LEADER;
}


RaftError
RaftFollower::_handleVoteRequest(
        const VoteRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(VoteResponse *)> &&cb)
{
    // Initialize with its original state.
    *stateChangeTo = m_type;
    return kevin::raft::_handleVoteRequest(
            req, m_raftGroup->m_metaStore, stateChangeTo, std::move(cb));
}


RaftError
RaftFollower::_handleVoteResponse(
        const VoteResponse &resp,
        RaftStateType *stateChangeTo)
{
    *stateChangeTo = m_type;
    RaftError ret = RaftError::RAFT_OK;
    CHANGE_TO_FOLLOWER_IF_TERM_HIGHER(resp, *stateChangeTo, ret);
    return ret;
}


RaftError
RaftFollower::_handleAppendLogRequest(
        AppendLogRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(AppendLogResponse *)> &&cb)
{
    // Initialized with errc = 0, term = 0.
    auto *resp = new AppendLogResponse();
    // Initialized with its original state.
    *stateChangeTo = m_type;

    auto *metaStore = m_raftGroup->m_metaStore;
    REJECT_LOWER_TERM(metaStore, req, *resp);

    RaftError ret = RaftError::RAFT_OK;
    CHANGE_TO_FOLLOWER_IF_TERM_HIGHER(req, *stateChangeTo, ret);
    if (unlikely(ret != RaftError::RAFT_OK)) {
        // If local term is lower, response leader with error and wait for retries.
        resp->set_error_code(static_cast<int32_t>(RaftError::RAFT_HIGHER_TERM));
        resp->set_term(metaStore->term_);
        resp->set_curr_term(metaStore->latestTerm_);
        resp->set_curr_lsn(metaStore->latestLsn_);
        cb(resp);
    }

    if (unlikely(
        req.prev_lsn() != metaStore->latestLsn_
        || req.prev_term() != metaStore->latestTerm_)) {
        // If the coming LRs is not continuous with local raft data store.
        resp->set_error_code(static_cast<int32_t>(RaftError::RAFT_LOG_MISSING));
        resp->set_term(metaStore->term_);
        resp->set_curr_term(metaStore->latestTerm_);
        resp->set_curr_lsn(metaStore->latestLsn_);
        cb(resp);
    }
    
    // Write LRs through raft data store.
    auto *dataStore = m_raftGroup->m_dataStore;
    const int sz = req.logs_size();
    std::vector<std::string> data;
    data.reserve(sz);
    for (int idx = 0; idx < sz; ++idx) {
        data.push_back(std::move(*req.mutable_logs(idx)));
    }
    dataStore->writeLogRecords(std::move(data), std::bind(cb, resp));

    return RaftError::RAFT_OK;
}


RaftError
RaftFollower::_handleAppendLogResponse(
        const AppendLogResponse &resp,
        RaftStateType *stateChangeTo)
{
    // This response must correspond to previous state. We only check the term.
    *stateChangeTo = m_type;
    RaftError ret = RaftError::RAFT_OK;
    CHANGE_TO_FOLLOWER_IF_TERM_HIGHER(resp, *stateChangeTo, ret);
    // Ignore other situations.
    return ret;
}


RaftError
RaftFollower::_handleElectionTimerExpired(int64_t term, RaftStateType *stateChangeTo)
{
    *stateChangeTo = m_type;
    auto *metaStore = m_raftGroup->m_metaStore;
    // Ignore lower term timer expirations.
    if (metaStore->term_ <= term) {
        metaStore->term_ = term; 
        *stateChangeTo = RaftStateType::RAFT_STATE_CANDIDATE;
        return RaftError::RAFT_OK;
    }
    return RaftError::RAFT_LOWER_TERM;
}


RaftError
RaftCandidate::_handleUserLog(const Log &log)
{
    return RaftError::RAFT_NOT_LEADER;
}


RaftError
RaftCandidate::_handleVoteRequest(
        const VoteRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(VoteResponse *)> &&cb)
{
    // Initialize with its original state.
    *stateChangeTo = m_type;
    return kevin::raft::_handleVoteRequest(
            req, m_raftGroup->m_metaStore, stateChangeTo, std::move(cb));
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
        AppendLogRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(AppendLogResponse *)> &&cb)
{
    // Initialized with errc = 0, term = 0.
    auto *resp = new AppendLogResponse();
    // Initialized with its original state.
    *stateChangeTo = m_type;

    auto *metaStore = m_raftGroup->m_metaStore; 
    RaftError ret = RaftError::RAFT_OK;
    CHANGE_TO_FOLLOWER_IF_TERM_HIGHER(req, *stateChangeTo, ret);
    // Set error in response.
    // If local term is higher, the leader will step down and will not retry.
    // If local term is lower, the leader will retry sending LRs from current
    // (term, lsn).
    resp->set_error_code(
            static_cast<int32_t>(RaftError::RAFT_NOT_FOLLOWER));
    resp->set_term(metaStore->term_);
    resp->set_curr_term(metaStore->latestTerm_);
    resp->set_curr_lsn(metaStore->latestLsn_);
    cb(resp);
    return RaftError::RAFT_OK;
}


RaftError
RaftCandidate::_handleAppendLogResponse(
        const AppendLogResponse &resp,
        RaftStateType *stateChangeTo)
{
    // This response must correspond to previous state. We only check the term.
    *stateChangeTo = m_type;
    RaftError ret = RaftError::RAFT_OK;
    CHANGE_TO_FOLLOWER_IF_TERM_HIGHER(resp, *stateChangeTo, ret);
    return ret;
}


RaftError
RaftCandidate::_handleElectionTimerExpired(int64_t term, RaftStateType *stateChangeTo)
{
    KEVIN_ASSERT(term < m_raftGroup->m_metaStore->term_,
            "Timer's term must be lower than current one.");
    return RaftError::RAFT_NOT_FOLLOWER;
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
    // Initialize with its original state.
    *stateChangeTo = m_type;
    return kevin::raft::_handleVoteRequest(
            req, m_raftGroup->m_metaStore, stateChangeTo, std::move(cb));
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
        AppendLogRequest &req,
        RaftStateType *stateChangeTo,
        std::function<void(AppendLogResponse *)> &&cb)
{
    // Initialized with errc = 0, term = 0.
    auto *resp = new AppendLogResponse();
    // Initialized with its original state.
    *stateChangeTo = m_type;

    RaftError ret = RaftError::RAFT_OK;
    CHANGE_TO_FOLLOWER_IF_TERM_HIGHER(req, *stateChangeTo, ret);
    resp->set_error_code(
            static_cast<int32_t>(RaftError::RAFT_NOT_FOLLOWER));
    resp->set_term(m_raftGroup->m_metaStore->term_);

    cb(resp);
    return RaftError::RAFT_OK;
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
RaftLeader::_handleElectionTimerExpired(int64_t term, RaftStateType *stateChangeTo)
{
    KEVIN_ASSERT(term < m_raftGroup->m_metaStore->term_,
            "Timer's term must be lower than current one.");
    return RaftError::RAFT_NOT_FOLLOWER;
}

} // namespace kevin
} // namespace raft 
