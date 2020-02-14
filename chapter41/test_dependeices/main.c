#include <stdio.h>

extern void init_value1(void);
extern int value1, value2;

int main(int argc, char *argv[])
{
    init_value1();
    printf("%d %d\n", value1, value2);
    return 0;
}
