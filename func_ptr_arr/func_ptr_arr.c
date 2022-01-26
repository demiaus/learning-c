#include <stdio.h>

int add(int a, int b) { return a + b; }
int sub(int a, int b) { return a - b; }
int mul(int a, int b) { return a * b; }
int div(int a, int b) { return a / b; }
int mod(int a, int b) { return a % b; }

int main(void)
{
        int a = 20;
        int b = 4;
        int f_n = 5;
        int (*f[])(int, int) = { add, sub, mul, div, mod };
        char *ops[] = {"+", "-", "*", "/", "%"};

        for (int i = 0; i < f_n; ++i) {
                printf("%d %s %d = %d\n", a, ops[i], b, f[i](a, b));
        }

    return 0;
}
