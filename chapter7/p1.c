#include "tlpi_hdr.h"

/*
[I] greyparrot@weed ~/P/T/chapter7 (master)> ./a.out 50 4096

Initial program break:          0x56470f2f9000
Allocating 50*4096 bytes 
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f2f9000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Program break is now:           0x56470f31a000
Freeing blocks from 1 to 50 in steps of 1
After free(), program break is: 0x56470f2f9000
*/

#define MAX_ALLOCS 1000000

int main(int argc, char *argv[])
{
    char *ptr[MAX_ALLOCS];
    int freeStep, freeMin, freeMax, blockSize, numAllocs, j;

    printf("\n");

    if(argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-allocs block-size [step [min [max]]]\n", argv[0]);

    numAllocs = get_int(argv[1], GN_NUM_NOT_ZERO, GN_ANY_BASE);

    if(numAllocs > MAX_ALLOCS)
        cmdLineErr("num-allocs > %d\n", MAX_ALLOCS);

    blockSize = get_int(argv[2], GN_NUM_NOT_ZERO, GN_ANY_BASE);

    freeStep = (argc > 3) ? get_int(argv[3], GN_NUM_NOT_ZERO, GN_ANY_BASE) : 1;
    freeMin  = (argc > 4) ? get_int(argv[4], GN_NUM_NOT_ZERO, GN_ANY_BASE) : 1;
    freeMax  = (argc > 5) ? get_int(argv[5], GN_NUM_NOT_ZERO, GN_ANY_BASE) : numAllocs;

    if(freeMax > numAllocs)
        cmdLineErr("free-max > num-allocs\n");

    printf("Initial program break:          %10p\n", sbrk(0));

    printf("Allocating %d*%d bytes \n", numAllocs, blockSize);
    for(j = 0;j < numAllocs; ++j){
        ptr[j] = malloc(blockSize);
        if(ptr[j] == NULL)
            errExit("malloc");
        printf("Program break is now:           %10p\n", sbrk(0));
    }

    printf("Program break is now:           %10p\n", sbrk(0));

    printf("Freeing blocks from %d to %d in steps of %d\n", freeMin, freeMax, freeStep);

    for(j = freeMin - 1; j < freeMax; j += freeStep)
        free(ptr[j]);

    printf("After free(), program break is: %10p\n", sbrk(0));
    
    return 0;
}
