/*
функции передается строка вида 

abcd,aaaaaa,qwerty,arctg

функция переставляет 1 и 2 слово местами, 3 и 4 и тд 
слов четное количество 
строка не пустая но может быть пустой (интересное... но вроде имелось в
виду не NULL но мб \0)

void swapstr(char *str);

ABC,mma,abc777,ebd 
mma,ABC,ebd,abc777 

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void
swapwords(char *words, int begin, int center, int end)
{
	int i;
	int len1 = center-begin, len2 = end-center-1;
	char *buf = (char*) malloc(len1*sizeof(char));

	for (i = 0; i < len1; i++)
	{
		buf[i] = words[begin+i];
	}

	for (i = 0; i < len2; i++)
	{
		words[begin+i] = words[center+1+i];
	}
	words[begin+i] = ',';
	for (i = 0; i < len1; i++)
	{
		words[begin+len2+1+i] = buf[i];
	}
	free(buf);
}


void 
swapstr(char *str)
{
	int i, len = strlen(str);
	if (len != 0)
	{
		int begin = 0, center = 0, end = 0;
		for (i = 0; i <= len; i++)
		{
			if (str[i] == ',' || str[i] == '\0')
			{
				if (center == 0)
				{
					center = i;
				}
				else if (end == 0)
				{
					end = i;
					swapwords(str, begin, center, end);
					begin = i + 1;
					center = 0;
					end = 0;
				}
			}
		}
	}
}


int main()
{
	// char a[19] = "ABC,mma,abc777,ebd";
	// char a[1] = "";
	char a[44] = "abcd,aaaaaa,qwerty,arctg,ABC,mma,abc777,ebd";
	printf("%s\n", a);
	swapstr(a);
	printf("%s\n", a);
	return 0;
}
