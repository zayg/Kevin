#pragma once

#include "proto/raft.pb.h"

namespace kevin {
namespace raft {

class RaftMetaStore {
public:
    /// persistent meta
    int64_t term_;
    // Replica id of the leader candidate that this meta store owner voted for.
    ReplicaId  votee_;

    /// volatile meta
    // Logs have been committed to users until committedIdx_ (included).
    int64_t committedLsn_ = 0;
    // Logs have been applied into RSM until appliedIdx_ (included).
    int64_t appliedLsn_ = 0;
};

class RaftDataStore {
public:
    inline RaftDataStore(bool isShadow)
        : shadow_(isShadow) {}

    inline bool isShadow() const {
        return shadow_;
    }

private:
    // Maximum LSN ever sent to this data store (the corresponding log may
    // not be persisted yet).
    int64_t maxLsn_ = 0;
    // Maximum LSN that all LSN before have been persisted to this data store.
    int64_t completedLsn_ = 0;

    // True if this is a shadow data store mapping to a remote follower.
    bool shadow_;
};

} // namespace kevin
} // namespace raft 
