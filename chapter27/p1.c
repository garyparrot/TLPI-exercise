/******************************************************************************
* File:             p1.c
*
* Author:           garyparrot  
* Created:          07/31/19 
* Description:      Exercise 1
*****************************************************************************/

#include "tlpi_hdr.h"

int main(int argc, char *argv[]){
    if(argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    execlp(argv[1], argv[1], "/", (char*)NULL);
    errExit("execlp");
}
