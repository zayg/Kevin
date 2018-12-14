## Considerations

### StateMachine
The state machine pattern will be suitable to express the raft state changes between candidate, follower and leader. For example, a server from FollowerState might transfer to CandidateState because of a TimerExpiredEvent. Special volatile states can be saved in the State object.

### EventQueue
Let's use event to represent any thing that may cause the server state changes. Consider that different events may come from local or remote simultaneously at any time, it will be easier to handle all the difficult state lock/wait issues if we just handle those events one by one. That's one reason we want to have a event queue.

Later on, supporting multiple raft groups can be done by expanding one EventQueue to multiple EventQueue and distributing raft groups into different queues. 

For the event types, based on what raft requires, we will have Log event, VoteEvent event, and HeartbeatExpired event at first.

### EventHandler
We will have different abstractions of EventHandlers for each event types. To benifit testing, we can mock the EventHandlers as well as event producers.
