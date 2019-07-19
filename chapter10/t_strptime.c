/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_strptime.c
*
* Author:           garyparrot
* Created:          2019/07/19
* Description:      Demostrating strptime
******************************************************************************/

#define _XOPEN_SOURCE /* strptime */
#include <time.h>     /* strptime strftime */
#include <locale.h>   /* setlocale */
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

char* parsing(const char* fmt, const struct tm* tmv){
    static char failed[] = "Parsing failed.";
    static char buf[1024];

    if(strftime(buf, 1024, fmt, tmv) != 0)
        return buf;
    else
        return failed;
}

int main(int argc, const char *argv[]){
    // strftime() transform tm into a string
    // strptime() transform string into tm structure
    // strptime won't update each field, better initialize all its field to zero before use it
    // and this function every set DST, mktime might be a work around.

    if(argc != 4) usageErr("%s target parse output", argv[0]);
    if(setlocale(LC_ALL, "") == NULL) errExit("setlocale");
    const char* tstr = argv[1];
    const char* pstr = argv[2];
    const char* ostr = argv[3];

    struct tm tms;

    memset(&tms, 0, sizeof(struct tm));

    if(strptime(tstr, pstr, &tms) == NULL) errExit("strptime");

    // clear DST
    tms.tm_isdst = -1;

    printf("Calendar time (seconds since Epoch): %ld\n", (long)mktime(&tms));

    char timestr[1024];
    if(strftime(timestr, 1024, ostr, &tms) == 0) errExit("strftime");

    printf("%s\n", timestr);

    
    return 0;
}
