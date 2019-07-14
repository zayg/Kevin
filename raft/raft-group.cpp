#include "raft/raft-group.h"
#include "raft/raft-state.h"
#include "raft/raft-log.h"
#include "raft/raft-store.h"

#include "proto/raft.pb.h"

using namespace kevin::raft;


template<typename T>
inline T
getEventPayload(void *payload)
{
    return static_cast<T>(payload);
}


// New INSERT event
RaftEvent *
RaftEvent::newEvent(const std::string &key, const std::string &value)
{
    char *buf;
    const size_t raftLogSize = sizeof(Log) + key.size() + value.size();
    auto raftEvent = RaftEvent::_newEvent(RaftEventType::USER_LOG, raftLogSize,
            &buf);

    // Initialize the Log.
    Log *log = reinterpret_cast<Log*>(raftEvent->parameter);
    log = new (buf + sizeof(RaftEvent)) Log(key.size(), RaftLogType::INSERT);

    // Append key and value to the Log.
    memcpy(KEY_PTR(log), key.data(), key.size());
    memcpy(VALUE_PTR(log), value.data(), value.size());

    return raftEvent;
}


RaftError
RaftGroup::addEvent(RaftEvent *event)
{
    this->m_events.push_back(event);
    // TODO(yihao) Throttling
    return RaftError();
}


RaftError
RaftGroup::write(const std::string &key, const std::string &value)
{
    RaftEvent* raftEvent = RaftEvent::newEvent(key, value);
    return this->addEvent(raftEvent);
}


RaftError
RaftGroup::read(std::string& key)
{
    // TODO(yihao)
    RaftError err;
    return err;
}


RaftError
RaftGroup::_handleUserLog(const Log &log)
{
    return m_state->_handleUserLog(log);
}


RaftError
RaftGroup::_handleVoteRequest(
        const VoteRequest &req,
        RaftStateType *stateChangeTo)
{
    return m_state->_handleVoteRequest(req, stateChangeTo);
}


RaftError
RaftGroup::_handleVoteResponse(const VoteResponse &resp)
{
    return m_state->_handleVoteResponse(resp);
}


RaftError
RaftGroup::_handleAppendLogRequest(const AppendLogRequest &req)
{
    return m_state->_handleAppendLogRequest(req);
}


RaftError
RaftGroup::_handleAppendLogResponse(const AppendLogResponse &resp)
{
    return m_state->_handleAppendLogResponse(resp);
}


RaftError
RaftGroup::_handleElectionTimerExpired()
{
    return m_state->_handleElectionTimerExpired();
}


RaftError
RaftGroup::_handleNewTerm(int64_t term)
{
    return m_state->_handleNewTerm(term);
}


