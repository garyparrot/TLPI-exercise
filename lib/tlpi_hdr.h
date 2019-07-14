#ifndef TLPI_HDR_H
#define TLPI_HDR_H

#include <sys/types.h>          /* Type definitions used by many programs */
#include <stdio.h>              /* Standard I/O functions */
#include <stdlib.h>             /* Prototypes of commonly used library functions
                                   plus EXIT_SUCCESS and EXIT_FAILURE constant */

#include <unistd.h>             /* Prototypes for many system calls */
#include <errno.h>              /* Declares errnno and defines error constants */
#include <string.h>             /* Commonly used string-handling functions */

#include "get_num.h"            /* Declares our functions for handling numeric
                                   arguments (getInt(), getLong()) */
#include "error_functions.h"    /* Declares our error-handling functions */

typedef enum { false = 0, true = 1} Boolean;

#define min(m,n) ((m) < (n) ? (m) : (n))
#define max(m,n) ((m) > (n) ? (m) : (n))

#endif
