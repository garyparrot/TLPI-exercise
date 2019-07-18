/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/18
* Description:      Chapter 8 Exercise 1
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <pwd.h>

// getpwnam store the result in a fixed place.
// every time it get called, it cover its previous result,
// first printf print the same name because its a string, printf accept the reference to same string
// second printf print different and corrent result, because that is not a reference. is a number. 
// the actual value get copy as argument for printf.

int main(int argc, const char *argv[]){

    // greyparrot greyparrot
    printf("%s %s\n",   (getpwnam("greyparrot")->pw_name), (getpwnam("root")->pw_name));
    // 666 0
    printf("%ld %ld\n", (long)(getpwnam("greyparrot")->pw_uid), (long)(getpwnam("root")->pw_uid));
    
    return 0;
}
