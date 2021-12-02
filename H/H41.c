/*
- Let's say, that length of each string is less than 32767
- Let's say, that strings couldn't be empty
- Function strcmp from <string.h> works as follows:
  - it returns 0, if strings are equal;
  - it returns 1,
    if code of the first different symbol of string1 is bigger 
    than code of the symbol of string2 or string1 is longer than string2
  - it returns -1,
    if code of the first different symbol of string1 is less 
    than code of the symbol of string2 or string1 is shorter than string2
*/

#include <stdio.h>

/*
- Arguments: pointers to the begging of the arrays (null position elements)
- !(*str1) and !(*str2) help us to indicate if one string ended
 (early exit: we don't look through the rest of symbols of longer string)
*/
int
str_cmp(char *str1, char *str2) {
    int result = 0;
    while (*str1 || *str2) {   // strings ends with null
        if ((*str1 < *str2) || !(*str1)) {
            return -1;
        } else if ((*str1 > *str2) || !(*str2)) {
            return 1;
        }
        str1++; 
        str2++;
    }
    return result;
}

int
main(void) {
    int MAX_LEN = __INT16_MAX__;
    char str1[MAX_LEN], str2[MAX_LEN];

    printf("%s\n%s", "Hello! Please, input strings", "String 1: ");
    scanf("%s", str1);

    printf("%s", "String 2: ");
    scanf("%s", str2);

    if (str_cmp(str1, str2) == 0) {
        printf("\n%s\n", "Strings are equal");
    } else {
        printf("\n%s\n", "Strings are NOT equal");
    }
    return 0;
}