/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/17
* Description:      www
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

// this array is pretty big but it doesn't mean the program is that much big.
// this array was place in the bss section
// Before we run this program, it just a record saying that there is a 
// BIGGGGG_ARRARY and its size is 1000000.
// 
// But once the program get running, it become a process, a instance of program.
// the bss record got its own memory space.
int BIGGGGG_ARRARY[1000000];

int main(int argc, const char *argv[]){
    
    return 0;
}
