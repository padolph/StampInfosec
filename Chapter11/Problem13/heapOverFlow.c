#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    int diff, size=8;
    char *buf1, *buf2;
    buf1 = (char *)malloc(size);
    buf2 = (char *)malloc(size);
    diff = buf2 - buf1;
    memset(buf2, '2', size);
    printf("BEFORE: buf2 = %s ", buf2);
    memset(buf1, '1', diff + 3);
    printf("AFTER: buf2 = %s\n", buf2);
    return 0;
}

