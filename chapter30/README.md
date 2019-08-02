# Chapter 30

## deadlock

There are two thread, and how dead lock might happens.

    1. [THREAD 1] acquire mutex1, succeed
    2. [THREAD 2] acquire mutex2, succeed 
    3. [THREAD 1] acquire mutex2, blocking
    4. [THREAD 2] acquire mutex1, blocking

In order to prevent dead lock.

1. Programmer can define a mutex hierarchy, thread have to acquire mutex1 and then mutex2. Only continue the work if both locks was acquired.
2. Try to acquire next lock. if failed, release all the lock I have. And try it later. This strategy is less frequency used. But It is more flexible and we don't need to define the mutex hierarchy.


