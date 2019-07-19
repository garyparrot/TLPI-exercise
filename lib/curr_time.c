#include <time.h>
#include "curr_time.h"

#define BUF_SIZE 1000


char* currTime(const char *format){
    static char buf[BUF_SIZE];

    time_t t = time(NULL);
    struct tm *current = localtime(&t);
    if(current == NULL) return NULL;

    size_t w = strftime(buf, BUF_SIZE, format, current);

    return (w != 0) ? buf : NULL;
}
