#pragma once
#include <common/error.h>

namespace kevin {
namespace consensus {

class VoteMeRequest;
class VoteMeResponse;
class AppendLogRequest;
class AppendLogResponse;
class IRaftState;

class RaftStateMachine {
public:
    kevin::common::Error handleVote(const VoteMeRequest &req, VoteMeResponse *resp);
    kevin::common::Error handleLog(AppendLogRequest &req, AppendLogResponse *resp);
    kevin::common::Error handleElectionTimerExpired();
private:
    IRaftState *mState = nullptr;
};

} // namespace kevin
} // namespace consensus
