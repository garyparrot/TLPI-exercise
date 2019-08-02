/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/08/02
* Description:      Exercise 1
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include <pthread.h>
#include "tlpi_hdr.h"

struct one_struct{
    int called;
    pthread_mutex_t mutex;
};

#define ONE_TIME_INITIALIZER { .called = 0, .mutex = PTHREAD_MUTEX_INITIALIZER }

void one_time_init(struct one_struct *control, void (*init)(void)){

    /* Ignore meanless lock & checking */
    if(control->called) return;

    /* acquire lock */
    pthread_mutex_lock(&(control->mutex));

    if(!control->called) init();
    control->called = 1;

    /* release lock */
    pthread_mutex_unlock(&(control->mutex));
}

struct one_struct night_call = ONE_TIME_INITIALIZER;

void nc(void){
    printf("Hi\n");
}

int main(int argc, const char *argv[]){

    one_time_init(&night_call, nc);
    one_time_init(&night_call, nc);
    one_time_init(&night_call, nc);
    one_time_init(&night_call, nc);
    one_time_init(&night_call, nc);
    
    return 0;
}
