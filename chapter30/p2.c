/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/08/02
* Description:      Exercise 2
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "search_tree.h"
#include "tlpi_hdr.h"

/* The global tree */
static struct tree t;

const char* keys[] = {
    "Hey",
    "I just wanna say",
    "Goodbye",
    "Goodbye to the people who hated on me",
    "Goodbye the people who loved, me",
    "Goodbye to the people who trusted me",
    "Goodbye goodbye to everybody",
    "Goodbye to the people who hated on me",
    "Goodbye the people who loved, me",
    "Goodbye to the people who trusted me",
    "Goodbye goodbye to everybody",
    "Now I need some changes in my life",
    "Now I need to move on in life",
    "Now is time to say goodbye",
    "I'm thinking deeply, right, now",
    "My life without you is like, ow",
    "I don't know how to explain, this",
    "I don't why why, it's happening to me",
    "But I just realised",
    "I'm not the person",
    "I used to be",
    "Now I want to leave This city",
    "This bullshit is not gonna happen again"
};
const size_t keylen = sizeof(keys) / sizeof(const char*);

void* thread1(void *args){
    // This thread keep query stuff
    int s;
    while(1) {
        for(int i = 0;i < keylen;i++){
            const char* val;
            if((s = lookup(&t, keys[i], (void**)&val)) && s > 0)
                errExitEN(s, "lookup");
            // if(s == -1) printf("Query stuff %s, not found. \n", keys[i]);
            if(s == 0 && strcmp(val, keys[i]) != 0)
                printf("Value inconsist %s\n Source: %s\n Exptect: %s\n", keys[i], val, keys[i]);
        }
    }

    return NULL;
}

void* thread2(void *args){
    // This thread keep remove stuff and insert it back after a while
    int s;
    while(1){
        for(int i = 0; i < keylen; i++){
            if((s = delete(&t, keys[i])) && s > 0)
                errExitEN(s, "delete");
            if((s = add(&t, keys[i], (void**)keys[i])) && s > 0)
                errExitEN(s, "add");
        }
    }
    return NULL;
}

int main(int argc, const char *argv[]){
    int s;

    if((s = initialize(&t)))
        errExitEN(s,"Cannot init tree");

    pthread_t t1,t2;

    for(int i = 0;i < keylen; i++)
        if((s = add(&t, keys[i], (void**)keys[i])) && s > 0)
            errExitEN(s, "add");

    if((s = pthread_create(&t1, NULL, thread1, NULL))) errExitEN(s, "thread1 pthread_create");
    if((s = pthread_create(&t2, NULL, thread2, NULL))) errExitEN(s, "thread1 pthread_create");

    for(;;);
    
    return 0;
}
