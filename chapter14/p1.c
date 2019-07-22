/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             p1.c
*
* Author:           garyparrot
* Created:          2019/07/22
* Description:      Chapter 14 Exercise 1
******************************************************************************/

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "tlpi_hdr.h"

int main(int argc, const char *argv[]){
    int random;
    if(argc < 3) 
        usageErr("%s amount destination [ inorder ]", argv[0]);
    if(argc == 4 && strcmp("inorder", argv[3]) == 0)
        random = 1;

    const int     sz = get_int(argv[1], GN_NUM_POSTIVE, GN_ANY_BASE);    
    const char *dest = argv[2];

    srand(time(NULL));

    int fd;
    char buff_name[1024];
    int *seq = (int*)malloc(sizeof(int) * sz);

    /* init creat sequence */
    for(int i = 0;i < sz;i++) seq[i] = i;
    /* stuff seq */
    for(int i = 0;i < sz && random;i++) {
        int a = rand() % sz;
        int b = rand() % sz;
        int c = seq[a]; seq[a] = seq[b]; seq[b] = c;
    }

    /* sync any pending modification before we continue */
    sync();
    struct timeval t0,t1,t2;

    gettimeofday(&t0, NULL);

    /* create files */
    for(int i = 0;i < sz;i++){
        snprintf(buff_name, 1024, "%s/x%6d", dest, seq[i]);
        if((fd = open(buff_name, O_WRONLY | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR)) == -1)
            errExit("open");
        if((write(fd, "A", 1)) != 1)
            errExit("write");
        if(close(fd) == -1)
            errExit("close");
    }

    sync();
    gettimeofday(&t1, NULL);

    /* delete files */
    for(int i = 0;i < sz;i++){
        snprintf(buff_name, 1024, "%s/x%6d", dest, i);
        if(unlink(buff_name) == -1)
            errExit("unlink");
    }

    sync();
    gettimeofday(&t2, NULL);


#define double_time(a) (double)(a.tv_sec * 1e6 + a.tv_usec)
    double creat_time = double_time(t1) - double_time(t0);
    double unlink_time= double_time(t2) - double_time(t1);
    double total_time = double_time(t2) - double_time(t0);

    printf("Creat Time  %5.2lf\n", creat_time / 1e6);
    printf("Delete Time %5.2lf\n", unlink_time/ 1e6);
    printf("Total Time  %5.2lf\n", total_time / 1e6);



    return 0;
}
