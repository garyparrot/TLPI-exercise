/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_setvbuf.c
*
* Author:           garyparrot
* Created:          2019/07/21
* Description:      demonstrating setvbuf, this function change the stdio's 
*                   buffering behavior.
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    // set buffer size 4069 bytes for stdout.
    // setvbuf will allocate that space for us since the buffer pointer is NULL.
    // _IOFBF stands for fully buffered.
    // _IOLBF stands for line buffered.
    // _IONBF stands for unbuffered.
    if(setvbuf(stdout, NULL, _IOFBF, 4069) != 0)
        errExit("setvbuf");

    // no return, this function is based on setvbuf
    // equivalent to setvbuf(fp, buf, (buf != NULL) ? _IOFBF : _IONBF, BUFSIZ)
    // the value of BUFSIZ defined in stdio.h, glibc defined it as 8192
    setbuf(stdout, malloc(BUFSIZ));

    // Now it is fully buffered, You won't see the string even though it get printed
    printf("Hello World\n");

    sleep(1);

    // force stdio stream flush its data(for stdout, call write()) to kerenl buffer cache.
    // If fflush() is perform on a input buffer, this caused all the data in buffer droped.
    // And it will eventually load data into input buffer until it is full or no more data.
    fflush(stdout);
    // fflush(NULL) this flush all the stdio buffer

    int value;
    scanf("%d", &value);
    printf("HEYHEY %d\n", value);
    // You better fflush(stdout) explicitly after you input any thing.
    // This will flush everything out of its content and prevent the terminal get messy.
    fflush(stdout);
    scanf("%d", &value);
    printf("HEYHEY %d\n", value);

    // After write some data, most of the program require its data get flush or at least get into kernel buffer cache
    // before doing other works.

    sleep(1);

    return 0;
}
