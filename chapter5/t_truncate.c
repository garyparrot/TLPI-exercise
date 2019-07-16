/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_truncate.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      truncate / ftruncate
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

// truncate will change the size of file to specified value.
// this system call require the name of the file, not a file descriptor.
//
// If the value is bigger than current size, the extra space will fill with null character.
// If the value is less than current size, data beyond the target size getting discarded.
//
// ftruncate function just like truncate, but instead of file name it accept a file descriptor as argument.
// ftruncate won't affect file descriptor's offset.

int main(int argc, const char *argv[]){

    off_t sz = sizeof(off_t) == 64 ? get_long(argv[2], GN_NOFLAG, GN_ANY_BASE) : get_int(argv[2], GN_NOFLAG, GN_ANY_BASE);
    truncate(argv[1], sz);

    return 0;
}
