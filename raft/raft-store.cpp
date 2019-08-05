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
RaftDataStore::writeLogRecords(
        std::vector<std::string> &&log,
        std::function<void ()> &&cb)
{
    return RaftError::RAFT_OK;
}
