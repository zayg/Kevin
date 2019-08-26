#pragma once

#include "raft/raft-common.h"
#include "proto/raft.pb.h"

#include <bitset>
#include <string>
#include <vector>

namespace kevin {
namespace raft {

class RaftMetaStore {
public:
    /// persistent meta
    int64_t term_;
    // Replica id of the leader candidate that this replica voted for.
    ReplicaId votee_;
    // The configuration of this raft group, the keys will be persistent in the
    // log. The value are local indexes of each replica.
    // TODO(yihao) hash function for ReplicaId
    std::unordered_map<ReplicaId, int8_t> group_;
    // The votes get from others for the current term. Indexed by the group_'s
    // values. votes_.count() will be the total vote count.
    // Need cleanup when term changed or raft state changed.
    std::bitset<1 << sizeof (int8_t)> votes_;


    /// volatile meta
    // current leader
    ReplicaId leader_;
    // Logs have been durable until (latestTerm_, latestLsn_) (included).
    int64_t latestTerm_ = 0;
    int64_t latestLsn_ = 0;
    // Logs have been applied into RSM until appliedIdx_ (included).
    int64_t appliedLsn_ = 0;

public:
    // Persist (term, votee) in a asynchronous way.
    // Cb is called at the last phase.
    kevin::raft::RaftError persistVotee(
            const int64_t term,
            const ReplicaId &candidate,
            std::function<void ()> &&cb);

    bool hasPersistedVotee(int64_t term);

    // Add Vote from voter.
    // @pre RaftState is RAFT_CANDIDATE.
    kevin::raft::RaftError addVote(const ReplicaId &voter);

    // Check if achieved majority votes.
    // @pre RaftState is RAFT_CANDIDATE.
    inline bool
    majorityVote() {
        return votes_.count() > group_.size() / 2;
    }

    inline bool
    staleThan(int64_t term, int64_t lsn)
    {
        return latestTerm_ < term || latestLsn_ <= lsn;
    }

    inline bool
    updateTerm(int64_t term)
    {
        if (term_ < term) {
            term_ = term;
            return true;
        }
        return false;
    }
};

class RaftDataStore {
public:
    inline RaftDataStore(bool isShadow)
        : shadow_(isShadow) {}

    inline bool isShadow() const {
        return shadow_;
    }

    kevin::raft::RaftError writeLogRecords(
            std::vector<std::string> &&log,
            std::function<void ()> &&cb);

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
