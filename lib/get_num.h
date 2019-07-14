#ifndef GET_NUM_H
#define GET_NUM_H

// I have to say the code in TLPI is a mess, terrible trash.

typedef enum { NOFLAG = 0x0000, NUM_POSTIVE = 0x0001, NUM_NOT_ZERO = 0x0002 } GN_FLAG;
typedef enum { BASE10 = 10, BASE8 = 8, BASE16 = 16, ANY_BASE = 0} GN_BASE;

long get_long(const char *arg, GN_FLAG flags, GN_BASE base);
int  get_int (const char *arg, GN_FLAG flags, GN_BASE base);

#endif
