#include <stdio.h>

extern int value1, value2, value3;

int main(int argc, char *argv[])
{
    printf("%d %d %d\n", value1, value2, value3);
    return 0;
}
