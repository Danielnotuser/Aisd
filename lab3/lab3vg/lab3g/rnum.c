#include <string.h>

#include "menu.h"

int read_val(int *n)
{
	int err;
	char end;
	do
	{
		err = scanf("%d%c", n, &end);
		if (err == -1) 
		{
			printf("\nInput has been interrupted.\n");
			return 1;
		}
		if ((err == 2) && (end == '\n'))
			return 0;
		printf("Error! Type of entered value is wrong.\n");
		printf("Write number again: ");
		scanf("%*[^\n]");
	} while (1);
}

int read_nat(int *n)
{
	int err;
	do
	{
		err = read_val(n);
		if (err)
			return 1;
		if (*n < 0)
			printf("Error! Number must be greater than or equal to zero.\n");
		else
			return 0;
		printf("Write number again: ");
	} while(1);
}

