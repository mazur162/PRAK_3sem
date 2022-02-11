#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void)
{
    char c;
    unsigned count = 0;
    unsigned max = 0;
    while ((c = getchar()) != EOF) {
        if ('a'<= c && c <= 'z') {
            if (c != 'q') {
                count++;
                if (count > max) {
                    max = count;
                }
            }
            else
            {
                count = 0;
            }
            printf("%u", count);
        } else {
            count = 0;
        }
    }
    printf("\n");
    printf("max = %u\n", max);
}