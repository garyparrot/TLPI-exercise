/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             long_jump.c
*
* Author:           garyparrot
* Created:          2019/07/17
* Description:      Demonstrate how to use set_jmp / long_jmp
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include <setjmp.h>
#include "tlpi_hdr.h"

/* AVOID TO USE SETJMP IF YOU CAN */

jmp_buf jmp;
int  fd_log;

void fatal_exit(){
    longjmp(jmp, 1);
}

void logging(const char *message){
    if(write(fd_log, message, strlen(message)) == -1)
        fatal_exit(); 
}

int main(int argc, const char *argv[]){

    // this program calling a function to write some data
    // to a file. if there is a exception, we jump to main
    // report the error and exit
    //
    // Be careful, don't forget to release allocated resource before long_jmp;
    
    // Don't handle error there :p
    
    // setjmp store register information. 
    // If compiler decide to optimize favorite_number so place it in register instead of memory.
    // Once the longjmp happened, this value might flash back.
    // add volatile qualifier to this variable so compiler won't optimize that.
    volatile int favorite_number = 666;

    fd_log = open(argv[1], O_WRONLY | O_APPEND);

    switch(setjmp(jmp))
    {
        case 0:
            logging("Hello Weed.\n");
            logging("HEY YOU.\n");
            logging("Welcome.\n");
            break;
        case 1:
            printf("SSSSSSSSheeep, the program is going to crash.\n");
            errExit("What the hell.");
            break;
    }

    return 0;
}
