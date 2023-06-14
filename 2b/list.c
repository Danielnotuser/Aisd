#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func.h"

#define DELIM "/"

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

void push(Queue *q, Man *a)
{
	if (q->tail)
	{
		(q->tail)->next = a;
		q->tail = a;
	}
	else
	{
		q->tail = a;
		q->head = a;
	}
}

void pop(Queue *q)
{
	if (q->head)
	{
		Man *a = (q->head)->next;
		free((q->head)->id);
		free(q->head);
		q->head = a;
	}
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

void queue_passed_time(Queue **arr_queue, int *time, int n, int curr_tm, int delta, FILE *out)
{
	int tm1;
	int curr;
	for (int i = 0; i < n; i++)
	{
		curr = curr_tm;
		Man *pers = arr_queue[i]->head;
		if (pers)
		{
			tm1 = delta;
			while (pers && (tm1 > 0))
			{
				if (tm1 - pers->ts >= 0)
				{
					tm1 -= pers->ts;
					curr += pers->ts;
					pop(arr_queue[i]);
					print_queue(arr_queue, n, curr, out);
					pers = arr_queue[i]->head;
				}
				else
				{
					pers->ts -= tm1;
					break;
				}
			}
			if (!arr_queue[i]->head)
				arr_queue[i]->tail = NULL;
			
		}
		if (delta > time[i])
			time[i] = 0;
		else
			time[i] -= delta;
	}
}
