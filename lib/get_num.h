#ifndef GET_NUM_H
#define GET_NUM_H

// I have to say the code in TLPI is a mess, terrible trash.

typedef enum { GN_NOFLAG = 0x0000, GN_NUM_POSTIVE = 0x0001, GN_NUM_NOT_ZERO = 0x0002 } GN_FLAG;
typedef enum { GN_BASE10 = 10, GN_BASE8 = 8, GN_BASE16 = 16, GN_ANY_BASE = 0} GN_BASE;

long get_long(const char *arg, GN_FLAG flags, GN_BASE base);
int  get_int (const char *arg, GN_FLAG flags, GN_BASE base);

#endif
