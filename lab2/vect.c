#include <stdlib.h>

#include "func.h"

#define MAX_LEN 30

Queue *queue_alloc()
{
	Queue *a = (Queue*) calloc(1, sizeof(Queue));
	if (!a)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return NULL;
	}
	a->arr = (Man**) calloc(MAX_LEN, sizeof(Man*));
	if (!a->arr)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return NULL;
	}
	a->head = 0;
	a->tail = 0;
	return a;
}

void free_queue(Queue *a)
{
	if (a->head <= a->tail)
	{
		for (int i = a->head; i < a->tail; i++)
		{
			free((a->arr)[i]->id);
			free((a->arr)[i]);
		}
	}
	else
	{
		for (int i = a->head; i < MAX_LEN; i++)
		{
			free((a->arr)[i]->id);
			free((a->arr)[i]);
		}
		for (int i = 0; i < a->tail; i++)
		{
			free((a->arr)[i]->id);
			free((a->arr)[i]);
		}
	}
	free(a->arr);
	free(a);
	
}

void pop(Queue *q)
{
	if ((q->arr)[q->head])
	{
		free((q->arr)[q->head]->id);	
		free((q->arr)[q->head]);
		q->head = (q->head + 1) % MAX_LEN;
	}
}

void push(Queue *q, Man *a)
{
	if (q->head != q->tail || !(q->arr)[q->head])
	{
		(q->arr)[q->tail] = a;
		q->tail = (q->tail + 1) % MAX_LEN;
	}	
}

void print_queue(Queue **a, int n, int tm, FILE* out)
{
	fprintf(out, "%d\n", tm);
	for (int i = 0; i < n; i++)
	{
		fprintf(out, "№%d ", i + 1);
		if (a[i]->head <= a[i]->tail || a[i]->head == 0)
		{
			for (int j = a[i]->head; j < a[i]->tail; j++)
			{
				fprintf(out, "%s ", (a[i]->arr)[j]->id);
			}
			fprintf(out, "\n");
		}
		else
		{
			for (int j = a[i]->head; j < MAX_LEN; j++)
			{
				fprintf(out, "%s ", (a[i]->arr)[j]->id);
			}
			for (int j = 0; j < a[i]->tail; j++)
			{
				fprintf(out, "%s ", (a[i]->arr)[j]->id);
			}
			fprintf(out, "\n");
		}
	}	
}

void queue_passed_time(Queue **a, int *time, int n, int curr_tm, int delta, FILE *out)
{
	int curr;
	for (int i = 0; i < n; i++)
	{
		curr = curr_tm;
		int tm1 = delta;
		for (int j = a[i]->head; j < a[i]->tail; j++)
		{
			if (((a[i]->arr)[j]->ts - tm1) > 0)
			{
				(a[i]->arr)[j]->ts -= tm1;
				break; 
			}
			else
			{
				tm1 -= (a[i]->arr)[j]->ts;
				curr += (a[i]->arr)[j]->ts;
				pop(a[i]);
				print_queue(a, n, curr, out);
			}
		}
		if (delta > time[i])
			time[i] = 0;
		else
			time[i] -= delta;
	}	
}
