/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p5.c
*
* Author:           garyparrot
* Created:          2019/07/21
* Description:      Exercise 5, implement tail command
******************************************************************************/

/*
 *
 * $ time ./noadvice.out -n 219719680 aaa > /dev/null
 * 
 * real	0m3.484s
 * user	0m3.211s
 * sys	0m0.272s
 *
 * $ time   ./advice.out -n 219719680 aaa > /dev/null
 * 
 * real	0m3.554s
 * user	0m3.206s
 * sys	0m0.284s
 *
 * Not sure what is going on, posix_fadvice didn't make it faster
 *
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char* const argv[] ){

    int line = 10;

    // Read arguments
    for(char c; (c = getopt(argc,argv, ":n:")) != -1;){
        switch(c){
            case 'n': line = get_int(optarg, GN_NUM_POSTIVE, GN_ANY_BASE); break;
            case ':': usageErr("%s [ -n line ] file", argv[0]); break;
            case '?': usageErr("%s [ -n line ] file", argv[0]); break;
        }
    }

    if(optind == argc) 
        usageErr("%s [ -n line ] file", argv[0]);
    /* fetch target filename */
    const char* filename = argv[optind];
    
    /* open file */
    int fd = open(filename, O_RDONLY ); 
    if(fd == -1) errExit("open");

    /* get length */
    off_t len = (long)lseek(fd, 0, SEEK_END);
    if(len == -1) errExit("lseek %d", __LINE__);

#define BUF_SIZE (1 << 14)
    char buffer[BUF_SIZE];

    /* Scan from the tail of file */
    off_t off;
    for(off = len; off && line ; ){
        long read_size = min(off, BUF_SIZE);               /* don't blow this file offset :p */
        off = lseek(fd, -read_size, SEEK_CUR);  
        if(off == -1) errExit("lseek %d", __LINE__);

        ssize_t sz = pread(fd, buffer, read_size, off );   /* read data */
        if(sz != read_size) errExit("pread");              /* I don't want to handle this. :p */

        /* posix_fadvice() advice for the next BUF_SIZE part of file */
#ifdef ADVISE
        posix_fadvise(fd, off - min(off, BUF_SIZE), min(off, BUF_SIZE), POSIX_FADV_WILLNEED);
#endif
        
        int lastCh = -1;
        // scanning buffer content and try to find newline character
        int i;
        for(i = read_size - 1; line > 0 && i >= 0; i--){

            char c = buffer[i];


            if(c == '\0' && lastCh == '\0') continue;      /* I hate file hole */

            if((c == '\n' || c == '\0') && lastCh == -1) continue;        /* ignore tail newline */

            if(c == '\n' || c == '\0'){                    /* there is a end of line */
                line -= 1;
            }

            lastCh = c;
        }

        // we find all the lines? adjust offset to correct position
        if(line == 0){
            off = off + i + 1 + 1;  /* plus 1 because for loop dec it, another 1 to escape newline character */
            break;
        }
    }
    /* after the loop, off should point to the right place */
    lseek(fd, off, SEEK_SET);
    int ln = 1;
    while(off < len){
        off_t   read_size = min(BUF_SIZE, len - off);
        ssize_t readed = read(fd, buffer, read_size);
        if(readed != read_size) errExit("read");
        ssize_t wrote = write(STDOUT_FILENO, buffer, readed);
        if(wrote != readed) errExit("write");

        off += read_size;
        ln += 1;
    }

    return 0;
}
