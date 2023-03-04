#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list_queue.h"

#define DELIM "/"

Man *man_alloc()
{
	Man *a = (Man*) calloc(1, sizeof(Man));
	if (!a)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return NULL;
	}
	return a;
}

Queue *queue_alloc()
{
	Queue *a = (Queue*) calloc(1, sizeof(Queue));
	if (!a)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return NULL;
	}
	a->head = NULL;
	a->tail = NULL;
	a->time = 0;
	return a;
}

Queue **arr_queue_alloc(int n)
{
	Queue **a = (Queue**) calloc(n, sizeof(Queue*));
	if (!a)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return NULL;
	}
	for (int i = 0; i < n; i++)
	{
		a[i] = queue_alloc();
		if (!a[i])
			return NULL;
	}
	return a;
}

void free_queue(Queue *a)
{
	Man *x = a->head;
	while (x)	
	{
		Man *y = x->next;
		free(x->id);
		free(x);
		x = y;
	}
	free(a);
}

void free_arr_queue(Queue **a, int n)
{
	for (int i = 0; i < n; i++)
		free_queue(a[i]);
	free(a);
}

void print_queue(Queue **arr_queue, int n, int tm, FILE *out)
{
	fprintf(out, "%d\n", tm);
	for (int i = 0; i < n; i++)
	{
		fprintf(out, "№%d ", i + 1);
		Man *pers = arr_queue[i]->head;
		Man *pers_next;
		while (pers)
		{
			fprintf(out, "%s ", pers->id);
			pers_next = pers->next;
			pers = pers_next;
		}
		fprintf(out, "\n");
	}
}

int queue_short(Queue **arr_queue, int n)
{
	int ind, min = 1000;
	for (int i = 0; i < n; i++)
	{
		if (!arr_queue[i]->head)
			return i;
		else if (arr_queue[i]->time < min)
		{
			min = arr_queue[i]->time;
			ind = i;
		}
	}
	return ind;	
}

void queue_passed_time(Queue **arr_queue, int n, int tm)
{
	int tm1;
	for (int i = 0; i < n; i++)
	{
		Man *pers = arr_queue[i]->head;
		if (pers)
		{
			tm1 = tm;
			while (pers)
			{
				if (tm1 - pers->ts > 0)
				{
					tm1 -= pers->ts;
					pers->ts = 0;
				}
				else
				{
					pers->ts -= tm1;
					break;
				}
				pers = pers->next;
			}
			pers = arr_queue[i]->head;
			Man *pers_next;
			while(pers && (pers->ts == 0))
			{
				pers_next = pers->next;
				free(pers->id);
				free(pers);
				arr_queue[i]->head = pers_next;
				pers = pers_next;
			}
			if (!arr_queue[i]->head)
				arr_queue[i]->tail = NULL;
			
		}
		if (tm > arr_queue[i]->time)
			arr_queue[i]->time = 0;
		else
			arr_queue[i]->time -= tm;
	}
}

int queue_init(int n, Man **arr_man, int len_man, FILE *out)
{
	Queue **arr_queue = arr_queue_alloc(n);
	if (!arr_queue)
		return 1;
	int curr_tm = 0;
	fprintf(out, "\n");
	for (int i = 0; i < len_man; i++)
	{
		print_queue(arr_queue, n, curr_tm, out);
		Man *a = arr_man[i];
		if (!curr_tm) 
			curr_tm = a->ta;
		else if (curr_tm != a->ta)
		{
			queue_passed_time(arr_queue, n, a->ta - curr_tm);
			curr_tm = a->ta;
		}
		int ind = queue_short(arr_queue, n);
		if (arr_queue[ind]->tail)
		{
			(arr_queue[ind]->tail)->next = a;
			arr_queue[ind]->tail = a;
		}
		else
		{
			arr_queue[ind]->tail = a;
			arr_queue[ind]->head = a;
		}
		arr_queue[ind]->time += a->ts;
	}
	print_queue(arr_queue, n, curr_tm, out);
	free_arr_queue(arr_queue, n);
	return 0;
}
