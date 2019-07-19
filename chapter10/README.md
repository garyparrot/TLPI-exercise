# Chapter 10 Time

* Calendar time: Time passed since specified point, in linux that will be Epoch.
* Elapsed: A period of time.

## Timezone

Timezone information store in /usr/share/zoneinfo.
There is a bunch of timezone setting over there, the one which defined your timezone was /etc/localtime.
That's is a symbolic link file which links to a timezone file.
usually under /usr/share/zoneinfo/ , I mean which genius put timezone file elsewhere.

Check tzfile(5) for the detail of that file, but you probably won't do that.

## Locale

Defined number / currency / date /  time format for your system.
All the locale information file locate at /usr/share/local (or /usr/lib/local maybe)
There is a lot of subdirectory named by this rule.

`` language[_territory[.codeset]][@modifier] ``

de\_DE.utf-8@euro means 
1. de -> Deutsch(德文)
2. DE -> German ISO Countery code
3. codeset -> utf-8 
4. euro -> using EUR(歐元)

## jiffies

Software clock Unit

這個是內核會透過 round-robin 分時調度算法分配CPU進程的單位。

It's size is defined by A Kernel constant - HZ.
It defined how often the timer interrupt will be.
Each interrupt will spend some time, during the period CPU can do nothing. So we can't set it too high.


