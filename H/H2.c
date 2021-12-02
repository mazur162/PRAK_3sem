#include <stdio.h>

enum { MAXLEN = 256 };

int
main(void)
{
    char str[MAXLEN + 1];
    int i;

    // reads line without overflowing str
    fgets(str, sizeof str, stdin);
    // expects a line with less than MAXLEN characters

    for (i = 0; i < sizeof str; ++i) {
        if (str[i] == '\n') {
            str[i] = '\0';
            break;
        }
    }
    int len = i;

    char t;
    for (i = 0; i < len / 2; ++i) {
        t = str[i]; 
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = t;
    }

    printf("%s\n", str);
}