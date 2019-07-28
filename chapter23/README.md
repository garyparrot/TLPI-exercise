# Chapter 23

## Precision of timer

Precision of software timer is limited by the jiffy(Software clock).

The smaller jiffy, the better precision. But in the mean time more CPU cycle wasted.

Linux kernel support high resolution timers, which doesn't limited by software clock.

Compile linux kernel with CONFIG-HIGH-RES-TIMER.

## ABSTIME

There is two way to measure a period of time

1. Relative : At beginning, measure the distance between target and current. And wait for the specific amount of time( e.g. 1min, 30sec )
2. Absolute : At beginning, record current time since a specific point(Epoch 123456), Then an loop keep query past time since specific point(Epoch 3456778). The distance between these number is the period

Sleep with first approach might be affected by kernel schedule. cause the waiting time is long-longer than expected(oversleeping).


