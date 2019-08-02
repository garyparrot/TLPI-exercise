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
#include <limits.h>
#include <pthread.h>

#include "tlpi_hdr.h"

static pthread_once_t once = PTHREAD_ONCE_INIT;

static pthread_key_t dirnameKey;
static pthread_key_t basenameKey;

void destructor(void *buf) { free(buf); }

void createKey(void){
    int s;
    if((s = pthread_key_create(&dirnameKey, destructor)))
        errExitEN(s, "pthread_key_create dirnameKey");
    if((s = pthread_key_create(&basenameKey, destructor)))
        errExitEN(s, "pthread_key_create basenameKey");
}

static const char *dirname(const char* path){
    int s, len, a;

    if(path == NULL) return ".";
    if((s = pthread_once(&once, createKey)))
        errExitEN(s, "pthread_once");

    /* Retrieve variable */
    char *buf = pthread_getspecific(dirnameKey);
    if(buf == NULL){
        buf = malloc(sizeof(char) * PATH_MAX);
        if(buf == NULL) errExit("malloc");

        if((s = pthread_setspecific(dirnameKey, buf)))
            errExitEN(s,"pthread_setspecific");
    }

    /* Copy string */
    for(len = 0, a = 0;len < PATH_MAX - 1 && path[a] != '\0'; a++)
        if(!(path[a] == '/' && path[a+1] == '/'))
            buf[len++] = path[a];
    buf[len] = '\0';

    if(buf[len-1] == '/') buf[--len] = '\0';

    char* c = strrchr(buf,'/');
    if(c != NULL && len > 1) *c = '\0';
    if(c == NULL) return ".";

    return buf;
}

static const char *basename(const char* path){
    int s, len, a;

    if(path == NULL) return ".";
    if((s = pthread_once(&once, createKey)))
        errExitEN(s, "pthread_once");

    /* Retrieve variable */
    char *buf = pthread_getspecific(basenameKey);
    if(buf == NULL){
        buf = malloc(sizeof(char) * PATH_MAX);
        if(buf == NULL) errExit("malloc");

        if((s = pthread_setspecific(basenameKey, buf)))
            errExitEN(s,"pthread_setspecific");
    }

    /* Copy string */
    for(len = 0, a = 0;len < PATH_MAX - 1 && path[a] != '\0'; a++)
        if(!(path[a] == '/' && path[a+1] == '/'))
            buf[len++] = path[a];
    buf[len] = '\0';

    if(buf[len-1] == '/') buf[--len] = '\0';

    char* c = strrchr(buf,'/');
    if(c != NULL && len > 1)
        return c+1;

    return buf;
}

int main(int argc, const char *argv[]){

    const char* path = (argc > 1) ? argv[1] : argv[0];

    printf("%s\n%s\n", dirname(path), basename(path));
    
    return 0;
}
