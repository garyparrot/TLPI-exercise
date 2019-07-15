#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

static inline char printable(char c) { return isprint((unsigned char)c) ? c : '#'; }

void parsing_error(const char* program_name, const char* errHint, char opt){
    if(errHint != NULL && opt != 0)
        fprintf(stderr, "%s (-%c)\n", errHint, printable(opt));
    fprintf(stderr, "Usage: %s [-a] filename\n", program_name);

    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    const char* program_name = argv[0];
    const char *dst_filename = NULL;
    Boolean append_file = false;

    for(int opt; (opt = getopt(argc, argv, ":a")) != -1; ){
        switch(opt){
            case 'a': append_file = true; break;
            case ':': parsing_error(program_name, "Missing argument", optopt);
            case '?': parsing_error(program_name, "Unknow option", optopt);
            default: fatal(__FILE__ ":%d Unexpected case in switch", __LINE__);
        }
    }
    if(optind + 1 == argc)
        dst_filename = argv[optind];
    else if(optind == argc)
        parsing_error(program_name, NULL, 0);
    else
        usageErr("Dummy syntax %s", argv[optind+1]);

    /* Open file */
    int fd;
    int flags = O_WRONLY | O_CREAT | (append_file ? O_APPEND : O_EXCL);
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP;
    if((fd = open(dst_filename, flags, mode)) == -1) {
        if(errno == EEXIST)
            usageErr("The file %s already exists.", dst_filename);
        else 
            errExit("open");
    }

#define BUF_SIZE 1024
    ssize_t readed;
    char buf[BUF_SIZE];
    while((readed = read(STDIN_FILENO, buf, BUF_SIZE)) > 0){
        for(ssize_t wrote = 0, w; wrote < readed; wrote += w)
            if((w = write(fd, buf + wrote, readed - wrote)) == -1)
                errExit("write");
        for(ssize_t wrote = 0, w; wrote < readed; wrote += w)
            if((w = write(STDOUT_FILENO, buf + wrote, readed - wrote)) == -1)
                errExit("write");
    }
    if(errno != 0) errExit("read");

    if(close(fd) == -1) errExit("close");

    return 0;
}
