#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int 
main(void)
{
	char c;
	int count = 0, maxx = 0, flag;
	
	while ((c = getchar()) && (c != EOF))
	{
		if ((c >= 'a') && (c <= 'z'))
		{
			if ((c != 'q') && flag) 
			{
				count++;
			}
			else
			{
				//printf("\n q %d\n", count);
				count = 0;
				flag = 0;
			}
		}
		else 
		{
			if (count > maxx) 
			{
				maxx = count;
			}
			//printf("\n ne bukva %d\n", count);
			count = 0;
			flag = 1;
		}
	}
	printf("\n%d\n", maxx);
}
