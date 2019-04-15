#include <stdio.h>

extern "C" int asm_func(int a, int b);

extern "C" int CSub(int a, int b)
{
    return a - b;
}

int main()
{
    int a = 0; int b = 0;
    scanf("%d%d", &a, &b);

    int c = asm_func(a, b);
    printf("main: CSub: %d - %d = %d\n", a, b, c);

    return 0;
}
