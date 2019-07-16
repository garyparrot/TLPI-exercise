/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             temp_file.c
*
* Author:           garyparrot
* Created:          2019/07/16
* Description:      mkstemp / tmpfile, creat temporary file.
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    // mkstemp: Creat a tmp file, you have to specify prefix name for the file
    //          at the end of the name require XXXXXX character, these character
    //          will be replace by a unique number.
    //          So make sure the given string is not const.
    //
    //          File created by mkstemp is owned by the user, no one could access it.
    //          Also the file is opened with O_EXCL flag to make sure the file is actually
    //          open by the program.
    //
    //          After the temp file been created, we usually call unlink(filename)
    //          to delete the name of temporary file from the file system.
    //          Once every reference to that file been released(e.g. close), the temp file 
    //          get deleted from the file system.
    //
    //          If the system call is success return value is a fd
    char tmpfile_name[] = "my_program_XXXXXX";
    if(mkstemp(tmpfile_name) == -1) errExit("mkstemp");
    if(unlink(tmpfile_name)  == -1) errExit("unlink");

    // tmpfile: unlike mkstemp, this function return a FILE* data struct.
    //          And tmpfile function will call unlink immediately, so we don't have to 
    //          handle these detail, cool.
    FILE* f = tmpfile(); 
    
    return 0;
}
