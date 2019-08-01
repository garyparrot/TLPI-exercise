/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             acct_view.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Showing process accounting data.
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/acct.h>
#include <limits.h>
#include <pwd.h>
#include <time.h>

#include "tlpi_hdr.h"

#define TIME_BUF_SIZE 100

const char *username(uid_t uid){
    struct passwd *pwd = getpwuid(uid);

    return pwd == NULL ? "Unknow" : pwd->pw_name;
}

static long long comptToLL(comp_t ct){
    long long mantissa = ct & 0x1fff;
    long long exp = (ct >> 13) & 0x7;

    return mantissa << (3 * exp);   /* aka (mantissa * 8^n) aka (mantissa * 2^3n) */
}

int main(int argc, const char *argv[]){

    if(argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s file\n", argv[0]);

    int acctFile = open(argv[1], O_RDONLY);
    if(acctFile == -1)
        errExit("open");
    
    printf("%-15s %-5s %-6s %-8s %-20s %15s %15s\n", "command", "flags", "term.", "user", "start time", "CPU", "elapsed");
    printf("%-15s %-5s %-6s %-8s %-20s %15s %15s\n", ""       , ""     , "status",""    , ""          , "time","time");

    struct acct_v3 ac;
    ssize_t numRead;
    char buf[TIME_BUF_SIZE];
    while((numRead = read(acctFile, &ac, sizeof(struct acct_v3))) > 0){
        if(numRead != sizeof(struct acct_v3))
            fatal("partial read");

        printf("%-15.15s ", ac.ac_comm);

        printf("%c", (ac.ac_flag & AFORK) ? 'F' : '-');
        printf("%c", (ac.ac_flag & ASU  ) ? 'S' : '-');
        printf("%c", (ac.ac_flag & AXSIG) ? 'X' : '-');
        printf("%c  ", (ac.ac_flag & ACORE) ? 'C' : '-');

#ifdef __linux__
        printf("%#6lx ", (unsigned long) ac.ac_exitcode);
#else
        printf("%#6lx ", (unsigned long) ac.ac_stat);
#endif

        const char *name = username(ac.ac_uid);

        printf("%-8.8s ", name);

        time_t t = ac.ac_btime;
        struct tm* loc = localtime(&t);

        if(loc == NULL){
            printf("%-20s", "Unknow time");
        }else{
            strftime(buf, TIME_BUF_SIZE, "%Y-%m-%d %T ", loc);
            printf("%20s ", buf);
        }

        printf("%15.2f %15.2f ", (double)(comptToLL(ac.ac_utime) + comptToLL(ac.ac_stime)) / sysconf(_SC_CLK_TCK),
                               (double)((ac.ac_etime)) / sysconf(_SC_CLK_TCK));
        printf("\n");
    }

    if(numRead == -1)
        errExit("read");

    exit(EXIT_SUCCESS);
}
