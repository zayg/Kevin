#pragma once
#include <common/error.h>

class IRaftState;
class VoteMeRequest;
class VoteMeResponse;
class AppendLogRequest;
class AppendLogResponse;

class RaftStateMachine {
public:
    Error handleVote(const VoteMeRequest &req, VoteMeResponse *resp);
    Error handleLog(AppendLogRequest &req, AppendLogResponse *resp);
    Error handleElectionTimerExpired();
private:
    IRaftState *mState = nullptr;
};
