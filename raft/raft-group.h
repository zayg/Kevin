#pragma once

#include "raft/raft-state.h"

#include <deque>
#include <vector>

namespace kevin {
namespace raft {

class RaftMetaStore;
class RaftDataStore;


enum class RaftEventType {
    USER_LOG,
    VOTE_REQUEST,
    VOTE_RESPONSE,
    APPEND_LOG_REQUEST,
    APPEND_LOG_RESPONSE,
    ELECTION_TIMEOUT,
    UPDATE_TERM
};


struct RaftEvent {
    static inline RaftEvent * _newEvent(RaftEventType tp,
            size_t parameterSize, char **buf)
    {
        *buf = new char[sizeof(RaftEvent) + parameterSize];
        return new (buf) RaftEvent(tp);
    }

    static inline RaftEvent * newEvent(
            const std::string &key,
            const std::string &value);

    inline RaftEvent(RaftEventType tp) : type(tp) {}

    RaftEventType type;
    char parameter[];
};


class RaftGroup : public RaftEventProcessor {
public:
    // user API
    inline kevin::raft::RaftError read(std::string& key);
    inline kevin::raft::RaftError addEvent(RaftEvent *event);

    inline kevin::raft::RaftError write(const std::string &key,
            const std::string &value);

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
            AppendLogRequest &req,
            RaftStateType *stateChangeTo,
            std::function<void(AppendLogResponse *)> &&cb) final;

    kevin::raft::RaftError _handleAppendLogResponse(
            const AppendLogResponse &resp,
            RaftStateType *stateChangeTo) final;

    kevin::raft::RaftError _handleElectionTimerExpired(
            int64_t term,
            RaftStateType *stateChangeTo) final;

private:
    std::deque<RaftEvent*> m_events;
    // Raft state. One of FOLLOWER, CANDIDATE, LEADER
    RaftEventProcessor *m_state = nullptr;

    // metadata for raft servers
    RaftMetaStore *m_metaStore = nullptr;
    RaftDataStore *m_dataStore = nullptr;
    // Shadow data stores of remote replicas.
    // Write requests are Batched inside.
    std::vector<RaftDataStore*> m_shadowStores;

    // All roles belong the RaftGroup will have private access.
    friend class RaftFollower;
    friend class RaftCandidate;
    friend class RaftLeader;
};

} // namespace kevin
} // namespace raft 
