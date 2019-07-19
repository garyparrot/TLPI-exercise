/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             ids.c
*
* Author:           garyparrot
* Created:          2019/07/19
* Description:      Manipulate UID/GID
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

void show_id(const char* text){
    printf("[%s] Real user id = %d\n" "Effective user id = %d\n" "Real group id = %d\n" "Effective group id = %d\n",
           text, getuid(), geteuid(), getgid(), getegid());
}

int main(int argc, const char *argv[]){

    show_id("Initial");

    // setuid change your user id,
    // For privilege user, this function change your the three id(real id, effective id, saved id) to the specified value.
    // That mean if privilege user do that, it will lose its privilege and cannot set it back(since it downgrade itself :p)
    //
    // If you are normal user, you can only change your id to either real id or saved id.
    // And every time you call it, only effective id changes.
    //
    // If privilege user want to downgrade itself temporarily and set it back later.
    // Better use seteuid()
    if(getuid() == 1000) setuid(1000);

    // seteuid, change effective id only.
    if(getuid() == 0){
        show_id("Before downgrade");
        seteuid(1000);
        show_id("Downgrade");
        seteuid(0);
        show_id("Set it back");
    }

    // setreuid(), this function not also change your effective id but also change your real id
    //
    // With the help of this function, you can make your real id different with your effective and saved id
    // For unprivileged user, only real id and effective id could be a vaild argument.
    //
    // this function also change your saved id to effective id at some condition 
    // If new_id != -1 : saved_id = new_effective_id
    // ElseIf new_effective_id != old_real_id : saved_id = new_effective_id
    setreuid(8888, 5555);
    show_id("setreuid");

    // getresuid/setresuid
    // not a part of SUSv3, It works on linux.
    // Change Real/Effective/Saved id 
    // Better never use it
    setresuid(1000,1000,1000);

    return 0;
}
