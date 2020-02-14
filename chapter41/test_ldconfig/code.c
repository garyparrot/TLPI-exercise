#include <stdio.h>

extern int int_plus1(int value);

int main(int argc, char *argv[])
{
    int a = int_plus1(0);
    printf("%d\n", a);
    return 0;
}
