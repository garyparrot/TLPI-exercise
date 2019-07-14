#include <stdarg.h>
#include "error_functions.h"
#include "tlpi_hdr.h"

#ifdef __GUNC__
__attribute__ ((__noreturn__))
#endif

// static function is seen only in the file it's declared in.
static void terminate(Boolean useExit3) {
    char *c = getenv("DUMP_THE_DAMN_CORE");
    if(c != NULL && *c != '\0') 
        abort();        // Stop this program without calling the functions which registed by atexit.
    else if(useExit3)
        exit(EXIT_FAILURE);         // exit program and atexit event, don't use this for subprocess
    else
        _exit(EXIT_FAILURE);        // exit program no atexit event.

    __builtin_unreachable();
}

static void outputError(Boolean useErr, int err, Boolean flushStdout, const char *fmt, va_list ap){

    #define BUF_SIZE 500
    char buf[2*BUF_SIZE+10], userMsg[BUF_SIZE], errText[BUF_SIZE];

    vsnprintf(userMsg, BUF_SIZE, fmt, ap);

    if(useErr)
    {
        const char *strerr = strerror(err);
        snprintf(errText, BUF_SIZE, "[%s]:", strerr != NULL ? strerr : "Unknow");
    }else
        snprintf(errText, BUF_SIZE, ":");

    snprintf(buf, 2*BUF_SIZE+10, "ERROR %s %s \n", errText, userMsg);

    if(flushStdout)
        fflush(stdout);

    fputs(buf,stderr);
    fflush(stderr);         /* In case stderr is not line-buffered */
}

void errMsg(const char *fmt, ...){
    va_list args;
    int savedErrno = errno;

    va_start(args, fmt);
    outputError(true, errno, true, fmt, args);
    va_end(args);

    errno = savedErrno;
}

void errExit(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    outputError(true, errno, true, fmt, args);
    va_end(args);

    terminate(true);
    __builtin_unreachable();
}

void err_exit(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    outputError(true, errno, false, fmt, args);
    va_end(args);

    terminate(false);
    __builtin_unreachable();
}

void errExitEN(int errnum, const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    outputError(true, errnum, true, fmt, args);
    va_end(args);

    terminate(true);
    __builtin_unreachable();
}

void fatal(const char *fmt, ...){
    va_list args;
    va_start(args, fmt);
    outputError(false, 0, true, fmt, args);
    va_end(args);

    terminate(true);
    __builtin_unreachable();
}

void usageErr(const char *fmt, ...){
    va_list args;
    fflush(stdout);

    fprintf(stderr, "Usage: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fflush(stderr);
    exit(EXIT_FAILURE);
}

void cmdLineErr(const char *fmt, ...){
    va_list args;

    fflush(stdout);     /* fflush before sending other content. */

    fprintf(stderr, "Command-line usage error: ");
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    fflush(stderr);
    exit(EXIT_FAILURE);
}
