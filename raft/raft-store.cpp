#include "raft/raft-store.h"
#include "proto/raft.pb.h"


using namespace kevin::raft;


kevin::raft::RaftError
RaftMetaStore::persistVotee(
        const int64_t term,
        const ReplicaId &candidate,
        std::function<void ()> &&cb)
{
    return RaftError::RAFT_OK;
}


bool
RaftMetaStore::hasPersistedVotee(int64_t term)
{
    return true;
}

RaftError
RaftMetaStore::addVote(const ReplicaId &voter)
{
    auto iter = group_.find(voter);
    if (iter == group_.end()) {
        // Cannot find voter in the raft group.
        return RaftError::RAFT_NOT_IN_MEMBERSHIP;
    }

    votes_.set(static_cast<size_t>(iter->second));

    return RaftError::RAFT_OK;
}


RaftError
RaftDataStore::writeLogRecords(
        std::vector<std::string> &&log,
        std::function<void ()> &&cb)
{
    return RaftError::RAFT_OK;
}
