#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int main(int argc, char *argv[])
{
    int fd;
    mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;

    /*
     * Long time ago, The implementation of open in UNIX cannot creat file(it only has two arguments).
     * For those years, people creat file by this function.
     *
     * Now open could creat file as the function do.
     * It use va_list and some little trick so that function support two form of argument.
     *
     */
    if((fd = creat(argv[1], mode)) == -1) errExit("creat");

    return 0;
}
