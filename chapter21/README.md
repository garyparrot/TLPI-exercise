# Chapter 21

## sig\_atomic\_t

Atomic integer variable

If you have to use global variable between main and signal handler, sig\_atomic\_t.

BTW, ``stdint.h`` defined the value range of sig\_atomic\_t.

[Read more about sig\_atomic\_t](https://stackoverflow.com/questions/24931456/how-does-sig-atomic-t-actually-work)

sig\_atomic\_t actually is a integer type that current platform guarantee any operation on it will be treat as a single instruction

Since it is a single instruction so there is no chance interrupt happening in the middle of operation.

## sigsetjump

setjump for signal handler.

## sigaltstack 

Alternative stack for signal
