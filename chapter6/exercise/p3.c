/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p3.c
*
* Author:           garyparrot
* Created:          2019/07/17
* Description:      Chapter 6 exercise 3
*                   This version doesn't use putenv/getenv. Might not be a 
*                   great implementation.
******************************************************************************/

// According to man page, the maximum size of args + environ was record at limits.h
// At my device, the value of ARG_MAX is 131072 bytes
//
// But sysconf(_SC_ARG_MAX) say it should be 2097152 ???

#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/limits.h>
#include "tlpi_hdr.h"

extern char** environ;

int setenv(const char* name, const char* value, int overwrite){
    // test if the name is valid
    if(name == NULL || name[0] == '\0' || strchr(name, '=') != NULL)
        return errno = EINVAL, -1;

    int len1 = strlen(name);
    int len2 = strlen(value); 

    // this macro indicate that the environ equal to specified name 
#define match_entry(envstr, name, len) (strncmp(envstr, name, len) == 0 && envstr[len] == '=')

    if(environ == NULL)
        environ = (char**)malloc(ARG_MAX), environ[0] = NULL;

    // test if the variable already there
    // assume nobody set environ to NULL.
    int m = 0;
    while(environ[m] != NULL && !match_entry(environ[m], name, len1)) ++m;
    if(environ[m] != NULL && overwrite == 0) return 0; /* don't overwrite it */ 

    // malloc space for it
    char* env = malloc(sizeof(char) * (len1 + len2 + 2));
    if(env == NULL) return errno = ENOMEM, -1;

    // generate the environ string
    int printed = sprintf(env, "%s=%s", name, value);
    assert(printed + 1 == (len1+len2+2));

    if(environ[m] == NULL){
        environ[m] = env;
        environ[++m] = NULL;
    }else{ /* we found a match in the previous while loop , and we have to overwrite it */
        environ[m] = env;
    }

    return 0;
}

int unsetenv(const char *name){
    if(environ == NULL) return 0;

    int len = strlen(name); 
    // tricky way to do this.
    // Since we have to delete some entry form it, there will be some vacant. We have to fill it with other env.
    // 
    // there is a point to the last vacant env entry 
    // if we found a match, delete it. update the pointer if possible
    // if we didn't found a match, test if there is a vacant waiting for fill.
    //      if so, fill it, vacant the no match entry, update lastVacant pointer to next vacant in linear time.
    // Time complexity might be O(n)
    int lastVacant = -1;
    for(int i = 0; environ[i] != NULL; i++)
        if(match_entry(environ[i], name, len)){
            if(lastVacant == -1) lastVacant = i;
            environ[i] = NULL;
        }else{
            // no match, test if there is other vacant wait for us to fill.
            if(lastVacant != -1){
                environ[lastVacant] = environ[i];
                environ[i] = NULL;
                // update vacant pointer in linear time.
                while(lastVacant <= i) 
                    if(environ[lastVacant] == NULL)
                        break;
                    else
                        lastVacant++;
                // no vacant entry found, set lastVacant to NULL.
                if(lastVacant == i + 1)
                {
                    lastVacant = -1;
                }
                
            }
        }
    return 0;
}
#undef match_entry

void list_env(){
    for(char** c = environ; *c != NULL ; c++)
        printf("%s\n", *c);
    puts("\n");
} 

int main(int argc, const char *argv[]){

    clearenv();

    setenv("USER", "Alex", 0);
    setenv("USER2", "Wolf", 0);
    setenv("USER", "JoeZ", 0);
    setenv("USER3", "AAA", 0);
    list_env();

    setenv("USER", "Joe", 1);
    list_env();

    unsetenv("USER");
    list_env();

    return 0;
}
