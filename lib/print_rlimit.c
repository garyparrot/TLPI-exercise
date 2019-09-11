#include <sys/resource.h>
#include "print_rlimit.h"
#include "tlpi_hdr.h"

int printRlimit(const char *msg, int resource){
    struct rlimit rlim;

    if(getrlimit(resource, &rlim) == -1)
        return -1;

    printf("%s soft=", msg);
    if(rlim.rlim_cur == RLIM_INFINITY)
        printf("infinite");
#ifdef RLIM_SAVED_CUR
    else if(rlim.rlim_cur == RLIM_SAVED_CUR)
        printf("unrepersentable");
#endif
    else
        printf("%lld", (long long) rlim.rlim_cur);

    printf("; hard=");
    if(rlim.rlim_max == RLIM_INFINITY)
        printf("infinite\n");
#ifdef RLIM_SAVED_CUR
    else if(rlim.rlim_max == RLIM_SAVED_CUR)
        printf("unrepersentable\n");
#endif
    else 
        printf("%lld\n", (long long) rlim.rlim_max);

    return 0;
}
