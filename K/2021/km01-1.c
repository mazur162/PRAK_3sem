/*
На стандартном потоке ввода подаются 64-битные беззнаковые числа в 
 шестнадцетеричной записи. На стандартный поток вывода напечатайте 
 64 числа в десятичной записи, каждое из который равно количеству 
 чисел во входных данных у которых на соответсвующей позиции бит 
 установлен. Первое число в выводе - это количество чисел во входных 
 данных, у которых установлен бит 0 (младший бит), а последнее число 
 в выводе - это количество чисел во входных данных, у которых установлен бит 63 (старший бит).
*/

#include <unistd.h>
#include <stdio.h>

enum
{
    COUNT = 64
};

int
main(void)
{
    int64_t number, number_modif;
    int bit_count[COUNT];
    
    for (int i = 0; i < COUNT; ++i)
    {
        bit_count[i] = 0;
    }

    while (scanf("%llx", &number) != EOF)
    {
        for (int i = 0; i < COUNT; ++i)
        {
            /* копируем значение number во вспомогательную переменную, чтобы не портить number */
            number_modif = number;
            if ((number_modif >> i) % 2 == 1)
            {
                bit_count[i] += 1;
            }
        }
    }
    
    for (int i = 0; i < COUNT; ++i)
    {
        printf("%d\n", bit_count[i]);
    }
    
    return 0;
}
