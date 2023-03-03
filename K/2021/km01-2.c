#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int 
compare(const void *arg1, const void *arg2)
{
    return strncmp((char *) arg1, (char *) arg2, 16);
    // char* a1 = (char *) arg1;
    // char* a2 = (char *) arg2;
    // int i;
    // for (i = 0; i < 16; i++) 
    // {
    //     if (*a1 < *a2)
    //     {
    //         return -1;
    //     }
    //     else if (*a1 > *a2)
    //     {
    //         return 1;
    //     }
    //     a1++;
    //     a2++;
    // }
    // return 0;
}


int 
main(int argc, char* argv[]) 
{
    long long len, i;
    //unsigned char q;
    unsigned char c[16];
    unsigned char* arr = (unsigned char *) malloc(33554432);
    FILE* f;
/*
    f = fopen(argv[1], "wb");
    for (q = 48; q > 0; q--)
    {
        fwrite(&q, 1, 1, f);
    }
    fclose(f);
    */
    f = fopen(argv[1], "rb");
    len = 0;
    while (fread(&c, 1, 16, f) == 16)
    {
        for (i = 0; i < 16; i++)
        {
            arr[len*16 + i] = c[i]; // memcpy(arr + len*16, c, 16)    
        }
        len++;
    }
    fclose(f);
    
    qsort(arr, len, 16, compare);

    f = fopen(argv[1], "wb");
    fwrite(arr, 1, len*16, f);
    fclose(f);

    return 0;
}
