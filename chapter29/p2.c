/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/08/01
* Description:      Exercise 2
******************************************************************************/

// Even though the main thread is exited, other thread could keep going.
// The problem is buf is defined in the main function, once the main thread exited
// The resource get released. That might cause unexpected behavior
//
// Solution: defined that struct somewhere else(glboal or allocated dynamically(Memory leak warning)).

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "tlpi_hdr.h"

struct someStruct { int weed; };

void *func(void *arg){
    struct someStruct *pbuf = (struct someStruct*)arg;

    printf("%d\n", pbuf->weed);

    return NULL;
}

int main(int argc, const char *argv[]){

    pthread_t t1;
    struct someStruct buf = { .weed = 666 };

    pthread_create(&t1, NULL, func, (void*)&buf);
    pthread_exit(NULL);
}
