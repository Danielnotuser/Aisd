#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#ifdef USE_LIST
#include "list_queue.h"
#else
#include "vect_queue.h"
#endif

#define DELIM "/"

int read_val(int *n, FILE *inp, FILE *out)
{
	int err;
	char end;
	do
	{
		err = fscanf(inp, "%d%c", n, &end);
		if (err == -1) 
		{
			fprintf(out, "\nInput has been interrupted.\n");
			return 1;
		}
		if ((err == 2) && (end == '\n'))
			return 0;
		fprintf(out, "Error! Type of entered value is wrong.\n");
		fprintf(out, "Write number again: ");
		fscanf(inp, "%*[^\n]");
	} while (1);
}

int read_nat(int *n, FILE *inp, FILE *out)
{
	int err;
	do
	{
		err = read_val(n, inp, out);
		if (err)
			return 1;
		if (*n <= 0)
			printf("Error! Number must be natural.\n");
		else
			return 0;
		fprintf(out, "Write number again: ");
	} while(1);
}

int check_buf(char *buf)
{
	int i = 0;
	if (buf[0] == '/')
	{
		fprintf(stderr, "Error! Wrong input.\n");
		return 1;
	}
	while(buf = strchr(buf, '/'))
	{
		i++;
		buf++;
	}
	if (i != 2)
	{
		fprintf(stderr, "Error! Wrong input.\n");
		return 1;
	}
	return 0;
}

int man_input(Man **a, char **buf)
{
	(*a)->id = strtok(*buf, DELIM);
	if (!(*a)->id)
	{
		fprintf(stderr, "Error! Wrong input.\n");
		free(*buf);
		free(*a);
		return 1;
	} 
	char *ptr = strtok(NULL, DELIM);
	if (!ptr) 
	{
		fprintf(stderr, "Error! Wrong input.\n");
		free(*buf);
		free(*a);
		return 1;
	}
	(*a)->ta = atoi(ptr);
	char *ptr1 = ptr;
	ptr = strtok(NULL, DELIM);
	if (!ptr) 
	{
		fprintf(stderr, "Error! Wrong input.\n");
		free(*buf);
		free(*a);
		return 1;
	}
	(*a)->ts = atoi(ptr);
	if (((*a)->ta <= 0) || ((*a)->ts <= 0))
	{
		fprintf(stderr, "Error! Wrong input.\n");
		free(*buf);
		free(*a);
		return 1;
	}
	(*a)->id = (char*) realloc((*a)->id, strlen((*a)->id) + 1);	
	return 0;
}

Man **man_init(int *len, FILE *inp, FILE *out)
{
	int err, i = 0;
	Man **arr_man = NULL;
	do
	{
		char *buf = (char*) calloc(81, sizeof(char));
		fprintf(out, "%d passenger: ", i);
		err = fscanf(inp, "%s", buf);
		if (err == -1) 
		{
			free(buf);
			break;
		}
		buf = (char*) realloc(buf, strlen(buf) + 1);
		if (check_buf(buf))
		{
			free(buf);
			continue;
		}
		Man *a = man_alloc();
		if (man_input(&a, &buf))
			continue;
		arr_man = (Man**) realloc(arr_man, (i + 1) * sizeof(Man*));
		if (!arr_man)
		{
			fprintf(stderr, "Error! Memory allocation error.");
			return NULL;
		}
		arr_man[i] = a;
		i++;
	} while (1);
	(*len) = i;
	if (i == 0) return NULL;
	return arr_man;
}

int man_cmp(const Man** a, const Man** b)
{
	return (*a)->ta - (*b)->ta;
}


