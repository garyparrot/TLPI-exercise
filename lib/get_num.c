#include "get_num.h"
#include "tlpi_hdr.h"
#include "limits.h"

static inline Boolean valid_base(GN_BASE base){
    return base == GN_BASE10 || base == GN_BASE8 || base == GN_BASE16 || base == GN_ANY_BASE;
}

#define handle_exception(message) do{ msg = message; goto exception; }while(0);
#define handle_exception_errno(message) do{ msg = message; goto exception_errno; }while(0);
static long to_long(const char* num, GN_FLAG flags, GN_BASE base){

    char *msg;

    errno = 0;
    long value = strtol(num, NULL, (int)base);

    if(errno != 0)
        handle_exception_errno("Exception during parsing string.")
    if(!valid_base(base)) 
        handle_exception("Invalid base.");
    if(flags & GN_NUM_POSTIVE && !(value >= 0))
        handle_exception("Invalid number, it should be postive."); 
    if(flags & GN_NUM_NOT_ZERO && !(value != 0)) 
        handle_exception("Invalid number, it shout not be zero.");

    return value; 

exception_errno:
    errExit(msg);
exception:
    fatal(msg);
}
#undef handle_exception
#undef handle_exception_errno

long get_long(const char *arg, GN_FLAG flags, GN_BASE base){ return to_long(arg, flags, base); }
int  get_int (const char *arg, GN_FLAG flags, GN_BASE base){ 
    long value = to_long(arg, flags, base); 
    if(!(INT_MIN <= value && value <= INT_MAX)) 
        fatal("Value is too large.");
    return value;
}

