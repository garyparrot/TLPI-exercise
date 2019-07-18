/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             crypt.c
*
* Author:           garyparrot
* Created:          2019/07/18
* Description:      Try crypt 
******************************************************************************/
#define _DEFAULT_SOURCE /* getpass */
#define _XOPEN_SOURCE /* crypt */

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include <pwd.h>
#include <shadow.h>

int main(int argc, const char *argv[]){

    // get username
    printf("username: "); 
    int len = sysconf(_SC_LOGIN_NAME_MAX); if(len == -1) len = 256;
    char* name = malloc(1 + len * sizeof(char));
    if(fgets(name, len, stdin) == NULL) errExit("fgets");
    // according to man page, fgets stop at EOF or newline. and newline will also in the string. we gotta remove it.
    if(strchr(name, '\n') != NULL) *strchr(name,'\n') = '\0';

    //get shadow record or passwd 
    struct passwd *pwd = getpwnam(name);
    if(pwd == NULL) fatal("No password record");

    struct spwd *spwdd = getspnam(name);
    if(spwdd == NULL && errno == EACCES) fatal("No permission");

    // this is the hash
    const char* password_hash = (spwdd != NULL) ? spwdd->sp_pwdp : pwd->pw_passwd;

    // getpass return a reference to user input password, the space is allocate statically.
    char* password = getpass("Password: ");

    char* encrypted = crypt(password, password_hash);

    // wipe out the god damn cleartext pwd;
    while(*password != '\0') *password++ = '\0';
    if(encrypted == NULL) errExit("crypt");

    printf("%s\n", (strcmp(encrypted, password_hash) == 0) ? "Welcome!":"Stay away.");
    
    return 0;
}
