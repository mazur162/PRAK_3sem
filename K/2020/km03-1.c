#include <stdio.h>
#include <stdlib.h>

int main (void)
{
    long long aa;
    unsigned long long a;
    unsigned b;
    unsigned long long sum = 0;
    scanf("%lld %u", &aa, &b);
    a = labs(aa);
    printf("%llu\n", a);
    printf("%u\n", b);
    while (a  != 0)
    {
        sum += a % b;
        printf("%llu\n", a % b);
        a = a /  b;  
    }
    printf("%llu\n", sum);

    return 0;
}