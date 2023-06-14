#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
		if ((err == 2) && (end == '\n' || end == ' '))
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
		{
			printf("\nInput has been intrerrupted.\n");
			return 1;
		}
		if (*n < 0)
			printf("Error! Number must be greater or equal to zero.\n");
		else
			return 0;
		printf("Write number again: ");
	} while(1);
}

int read_arr_nat(int **arr, int num)
{
	(*arr) = (int*) calloc(num, sizeof(int));
	for (int i = 0; i < num; i++)
		if (read_nat(&(*arr)[i]))
			return 1;
	return 0;
}

int read_str(char** s)
{
	char buf[81];
	char *res = NULL;
	int n, buf_len, res_len, last_len = 0;
	do
	{
		n = scanf("%80[^\n]", buf);
		if (n < 0)
		{
			printf("\nInput has been intrerrupted.\n");
			return 1;
		}
		else if (n > 0)
		{
			buf_len = strlen(buf);
			res_len = last_len + buf_len;
			res = (char*) realloc(res, res_len + 1);
			memcpy(res + last_len, buf, buf_len);
			last_len = res_len;
		}
		else
			scanf("%*c");
	} while (n > 0);
	if (last_len > 0)
	{
		res[last_len] = '\0';
	}
	else
		res = calloc(1, sizeof(char));
	(*s) = res;
	return 0;
}
