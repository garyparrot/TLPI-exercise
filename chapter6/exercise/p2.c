/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p2.c
*
* Author:           garyparrot
* Created:          2019/07/17
* Description:      
******************************************************************************/

#include <setjmp.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

jmp_buf buf;

// it crashed.

void jump_there(){
    setjmp(buf);
    printf("WEED\n");
}

int main(int argc, const char *argv[]){

    jump_there();

    longjmp(buf,1);
    
    return 0;
}
