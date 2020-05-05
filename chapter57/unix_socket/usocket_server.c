#include "usocket.h"

#define BACKLOG 5

char buffer[BUF_SIZE];

int main(int argc, char *argv[])
{
    /* create socket */
    int sockfd;
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        errExit("socket");

    /* Delete sock file if already exists */
    if(remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
        errExit("remove~%s", SV_SOCK_PATH);

    /* binding address */
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        errExit("bind");

    /* listening */
    if(listen(sockfd, BACKLOG) == -1)
        errExit("listen");

    for(;;){

        /* Accept a connection */
        int clientfd;
        if((clientfd = accept(sockfd, NULL, NULL)) == -1)
            errExit("accept");

        /* read data, print on screen */
        ssize_t numRead;
        while((numRead = read(clientfd, buffer, BUF_SIZE)) > 0)
            if(write(STDOUT_FILENO, buffer, numRead) != numRead)
                fatal("partial/failed write");

        if(numRead == -1)
            errExit("read");
        if(close(clientfd) == -1)
            errMsg("close");
    }

    return 0;
}
