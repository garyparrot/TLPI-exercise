#include "usocket.h"

char buf[BUF_SIZE];

int main(int argc, char *argv[])
{

    /* create socket */
    int sockfd;
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
        errExit("socket");

    /* construct server address */
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    /* connect */
    if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        errExit("connect");

    /* stdin to sock */
    ssize_t numRead;
    while((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
        if(write(sockfd, buf, numRead) != numRead)
            fatal("partial/failed write");

    if(numRead == -1)
        errExit("read");

    return 0;
}
