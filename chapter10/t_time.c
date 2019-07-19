/******************************************************************************
*                    The Linux Programming Interface practices.
* File:             t_time.c
*
* Author:           garyparrot
* Created:          2019/07/19
* Description:      Demonstrating how to get time on linux.
******************************************************************************/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#include "curr_time.h"
#include <time.h>           /* time() */
#include <sys/time.h>       /* gettimeofday() */

typedef long long int ll;

int main(int argc, const char *argv[]){

    // there is two way to get time on linux

    time_t t = time(NULL);      // return the passed second since Epoch.
    time(&t);                   // another way to call this function, result will be store in the specified variable.

    struct timeval tv;
    // tv is a structure contain these members
    //      time_t      tv_sec      // Seconds
    //      suseconds_t tv_usec     // Microseconds
    // Another variable will always be NULL
    // Long time ago, second argument stands for timezone info. Now it is deprecated.
    if(gettimeofday(&tv, NULL) == -1)
        errExit("gettimeofday");
    
    // ctime return a string of current time, it consider timezon
    // the space is allocated statically during runtime.
    // there is a newline character at tail.
    printf("%s", ctime(&(tv.tv_sec)));

    // ctime() gmtime() localTime() asctime() share the same mem space

    // gmtime() transform calendar time into UTC time directly
    t = time(NULL);
    struct tm *broken_down_time = gmtime(&t);
    printf("%d/%d/%d %d:%d\n",
            broken_down_time->tm_year + 1900,
            broken_down_time->tm_mon,
            broken_down_time->tm_mday,
            broken_down_time->tm_hour,
            broken_down_time->tm_min);

    // localtime() transform calendar time into UTC time, but it will consider timezone and DST.
    t = time(NULL);
    struct tm *broken_down_time_tz = localtime(&t);
    printf("%d/%d/%d %d:%d\n",
            broken_down_time_tz->tm_year + 1900,
            broken_down_time_tz->tm_mon,
            broken_down_time_tz->tm_mday,
            broken_down_time_tz->tm_hour,
            broken_down_time_tz->tm_min);
    // tm->tm_sec between 0 to 60 because it consider leap second.
    
    // mktime() transform broken-down time into calendar time.
    // this function might modify the speciied tm structure.
    // Each filed could be out of its usual range. 
    // e.g. if you chagne second to 66666, it will try to make the
    // value between the range it should be and modify other field to exact time.
    // This is insame.
    //
    // mktime consider DST
    // if tm.tm_isdst
    //      > 0 : transform broken_down_time to calendar time as DST is enabled
    //      = 0 : transform broken_down_time to calendar time as DST is disabled
    //      < 0 : before transform it, try to identify is that time DST.
    //
    // mktime consider timezone. 
    time_t original = mktime(broken_down_time_tz);
    broken_down_time_tz->tm_sec += 60;      /* plus 60 sec */
    time_t after1m  = mktime(broken_down_time_tz);

    printf("%lld - %lld = %lld\n", (ll)after1m, (ll)original, (ll)after1m - (ll)original);

    // asctime() transfrom broken-down time to string
    // doesn't consider timezone since tm broken-down time should handle this when it get created.
    printf("%s", asctime(broken_down_time_tz));

    printf("%s", currTime("%A %B"));

    return 0;
}
