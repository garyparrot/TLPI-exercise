# Chapter 22

Signal can be regard as a IPC method.

* Signal have reentrant, race condition, global variable issue. ( If make use of sigwaitinfo() or signalfd(), that might not be a big problems. ) 
* Standard signal won't queue requests, even though Real time signal do. There is the amount limit for queue.
* Transmission rate is slow, one integer and one pointer for each signal.

For these reason, people usually don't use signal as a IPC method.

Modern usage for Signal: Process Synchronization, Event notification, Job control, Timer

## Exercise

### Exercise 3

Someday I will do it
