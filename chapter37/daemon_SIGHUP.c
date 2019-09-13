/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             daemon_SIGHUP.c
*
* Author:           garyparrot
* Created:          2019/09/12
* Description:      Demonstrate how to reinitialize this daemon via SIGHUP
******************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>

#include "becomeDaemon.h"

#include "tlpi_hdr.h"

static const char *LOG_FILE    = "/tmp/ds.log";
static const char *CONFIG_FILE = "/tmp/ds.conf";
static int fdlog;

static volatile sig_atomic_t hupReceived = 0;

static void sighupHandler(int sig){ hupReceived = 1; }

static void logOpen(const char *log_file){
    if((fdlog = open(log_file, O_RDWR | O_CREAT, S_IRWXU)) == -1) {
        exit(EXIT_FAILURE);
    }
    dprintf(fdlog, "Open log file\n");
}
static void logClose(){ 

    dprintf(fdlog, "Close log file\n");

    if(close(fdlog) == -1)
        errExit("close log file failed"); 
}

static void readConfigFile(const char *config){

    int fdconfig;
    char buffer[1024];
    ssize_t readlen;

    if((fdconfig = open(config, O_RDONLY)) == -1){
        dprintf(fdlog, "Cannot open configuration\n");
        return;
    }

    dprintf(fdlog, "Read configuration : ");

    /* print config file content */
    while((readlen = read(fdconfig, buffer, 1024)) > 0)
        dprintf(fdlog, "%s", buffer);

    /* indicate the result */
    if(readlen == -1) {
        dprintf(fdlog, "Failed to read configuration\n");
        return;
    }else{
        dprintf(fdlog, "\n");
    }

    if(close(fdconfig) == -1)
        dprintf(fdlog, "Failed to close configuration file\n");
}

static void logMessage(const char *fmt, ...){
    va_list ls;
    va_start(ls, fmt);
    vdprintf(fdlog, fmt, ls);
    va_end(ls);
}

int main(int argc, const char *argv[]){

    const int SLEEP_TIME = 5;    
    int count = 0;
    int unslept;
    struct sigaction sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sighupHandler;
    if(sigaction(SIGHUP, &sa, NULL) == -1)
        errExit("sigaction");

    printf("Ready to become daemon\n");

    if(becomeDaemon(0) == -1)
        errExit("becomeDaemon");

    logOpen(LOG_FILE);
    readConfigFile(CONFIG_FILE);

    unslept = SLEEP_TIME;

    for(;;){
        unslept = sleep(unslept);

        if(hupReceived) {
            logClose(); 
            logOpen(LOG_FILE);

            readConfigFile(CONFIG_FILE);
            hupReceived = 0;
        }

        if(unslept == 0){
            count++;
            logMessage("Main: %d\n", count);
            unslept = SLEEP_TIME;
        }

        fsync(fdlog);

    }
    
    return 0;
}
