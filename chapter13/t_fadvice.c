/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_fadvice.c
*
* Author:           garyparrot
* Created:          2019/07/21
* Description:      demonstrating posix_fadvice(), given your kernel advice.
*                   Advice about how you access storage device, they will used
*                   difference strategy to manage kernel buffer cache(if supported)
******************************************************************************/


#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  /* posix_fadvice */
#include <unistd.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){

    // posix_fadvice(int fd, off_t offset, off_t len, int advice)
    //  given kernel advice, range [offset, offset+len]
    //  if len equal to 0, that means from offset to the end of file
    
    /* Default behavior */
    posix_fadvice(STDOUT_FILENO, 0, 0, POSIX_FADV_NORMAL);

    /* we will read data from lower offset to high offset */
    posix_fadvice(STDOUT_FILENO, 0, 0, POSIX_FADV_SEQUENTIAL);
    
    /* process will read specific area later */
    /* kernel cannot guarantee how long these space will be there for you */
    /* if the system have memory shortage, they might dispose it really quick. */
    /* so better finsihed your task after the call fast. */
    posix_fadvice(STDOUT_FILENO, 0, 0, POSIX_FADV_WILLNEED);
    
    /* Don't cache specific area */
    /* If there is data(specific area) in cache, kernel will apply these data first */
    /* We are not able to know does kernel finsihed this job, better do it ourself by call
     * sync() or fdatasync() . */
    posix_fadvice(STDOUT_FILENO, 0, 0, POSIX_FADV_DONTNEED);

    return 0;
}
