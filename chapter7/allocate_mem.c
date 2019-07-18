/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             allocate_mem.c
*
* Author:           garyparrot
* Created:          2019/07/18
* Description:      Demo for allocating memory in C
******************************************************************************/

#include <malloc.h>  /* memalign */
#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    int *mem;
    // malloc memory
    // free doesn't decrease program break every time it get called.
    // even you release all the memory, it might not release everything it got.
    mem = malloc(sizeof(int) * 100); free(mem);

    // allocate memory, first is the amount of item, second is the size of each item.
    mem = calloc(100, sizeof(int));

    // re-allocate memory, if the size is bigger than current the content will be preserved.
    // sometime, realloc is faster than we did it manually.  [allocate new space] -> [copy content] -> [free old space]
    // because realloc will check if there is available space in front of the old space.
    // this function will prevent actually get a bigger space if possible.
    mem = realloc(mem, sizeof(int) * 1000); 
    free(mem);

    // memalign : aligned memory, useful for some situation.
    // the aligned value should be multiple of 2
    int *amem = memalign(1024, 4096);
    printf("amem locate at %lld(%lld %% 1024 = %lld)\n", (long long)(amem), (long long)(amem), ((long long)(amem)) % 1024);
    free(amem);

    // not every UNIX implement memalign, SUSv3 announce a similar function called posix_memalign()
    // the return value of posix_memalign is a errno number, not -1. 
    int err = posix_memalign((void**)&amem, 1024, 4096);
    printf("amem locate at %lld(%lld %% 1024 = %lld)\n", (long long)(amem), (long long)(amem), ((long long)(amem)) % 1024);
    free(amem);

    // allocate space on stack
    // This stuff is similar to VLA(Variable Length Array) which could allocate dynamic size of array during runtime.
    // BUT BE CAREFUL, its life cycle not like usual variable which lives in its block space
    // Its life cycle is the whole function.
    //
    // If you try to put this line in a for-loop. keep calling that. 
    // That might blow your stack since each value stay there until the function getting exit.
    mem = alloca(sizeof(int) * 1000);
    // no need to free it.

    return 0;
}
