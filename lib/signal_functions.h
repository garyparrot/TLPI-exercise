#ifndef HEADER_GUARD_SIGNAL_FUNCTIONS
#define HEADER_GUARD_SIGNAL_FUNCTIONS 

#include <stdio.h>
#include <signal.h>

void printSigset(FILE*, const char*, const sigset_t*);
int printSigMask(FILE*, const char*);
int printPendingSigs(FILE*, const char*);

#endif /* ifndef HEADER_GUARD_SIGNAL_FUNCTIONS */
