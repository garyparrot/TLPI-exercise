/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_open_sync.c
*
* Author:           garyparrot
* Created:          2019/07/21
* Description:      Demonstrating speed influence when O_SYNC is enabled.
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/times.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

// According observation, the speed of O_SYNC is way to slow.

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    if(argc != 3) usageErr("%s { sync | dsync | no } bufsize", argv[0]);
    
    // O_SYNC flag makes forces file integrity.
    // So every data and metadata will write to the storage device
    // O_SYNC   file integrity.
    // O_DSYNC  data integrity.
    // O_SYNC  | O_RSYNC  act like file sync before reading
    // O_DSYNC | O_RSYNC act like data sync before reading
    int sync_flag = 0;
    sync_flag = (strcmp(argv[1], "sync") == 0) ? O_SYNC  : 0;
    sync_flag = (strcmp(argv[1], "dsyn") == 0) ? O_DSYNC : 0;

    int fd = open("tempfile", O_WRONLY | sync_flag | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR);
    if(fd == -1) errExit("open");


    size_t total = 1 << 16; 
    size_t bufsiz = get_int(argv[2], GN_NOFLAG, GN_ANY_BASE);
    char *buf = malloc(sizeof(char) * bufsiz);

    struct timeval t1,t2;

    printf("Begin writing\n");
    gettimeofday(&t1, NULL);

    while(total > 0){
        size_t w = min(total, bufsiz);
        if(write(fd, buf, w) == -1)
            errExit("write");
        total -= w;
    }

    gettimeofday(&t2, NULL);
    printf("Done\n");

    long tt = (t2.tv_sec * 1000000 + t2.tv_usec) - (t1.tv_sec * 1000000 + t1.tv_usec);
    printf("%lf\n", (double)tt / 1000000);

    close(fd);
    
    return 0;
}
