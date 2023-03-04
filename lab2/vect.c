#include <stdlib.h>
#include "vect_queue.h"

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
Queue *queue_alloc(int len_man)
{
	Queue *a = (Queue*) calloc(1, sizeof(Queue));
	if (!a)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return NULL;
	}
	a->arr = (Man**) calloc(len_man, sizeof(Man*));
	if (!a->arr)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return NULL;
	}
	a->time = 0;
	a->head = 0;
	a->tail = 0;
	return a;
}
Queue **arr_queue_alloc(int n, int len_man)
{
	Queue **a = (Queue**) calloc(n, sizeof(Queue*));
	if (!a)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return NULL;
	}
	for (int i = 0; i < n; i++)
	{
		a[i] = queue_alloc(len_man);
		if (!a[i])
		{
			return NULL;
		}
	}
	return a;
}

void free_queue(Queue *a)
{
	for (int i = a->head; i < a->tail; i++)
	{
		free((a->arr)[i]->id);
		free((a->arr)[i]);
	}
	free(a->arr);
	free(a);
	
}
void free_arr_queue(Queue **a, int n)
{
	for (int i = 0; i < n; i++)
	{
		free_queue(a[i]);
	}	
	free(a);
}

void print_queue(Queue **a, int n, int tm, FILE* out)
{
	fprintf(out, "%d\n", tm);
	for (int i = 0; i < n; i++)
	{
		fprintf(out, "№%d ", i + 1);
		for (int j = a[i]->head; j < a[i]->tail; j++)
		{
			fprintf(out, "%s ", (a[i]->arr)[j]->id);
		}
		fprintf(out, "\n");
	}	
}

int queue_short(Queue **a, int n)
{
	int minn = 1000, ind;
	for (int i = 0; i < n; i++)
	{
		if (a[i]->time < minn)
		{
			minn = a[i]->time;
			ind = i;
		}
	}	
	return ind;
}

void queue_passed_time(Queue **a, int n, int tm)
{
	for (int i = 0; i < n; i++)
	{
		int head_delta = 0;
		int tm1 = tm;
		for (int j = a[i]->head; j < a[i]->tail; j++)
		{
			if (((a[i]->arr)[j]->ts - tm1) > 0)
			{
				(a[i]->arr)[j]->ts -= tm1;
				break; 
			}
			else
			{
				head_delta++;
				tm1 -= (a[i]->arr)[j]->ts;
				free((a[i]->arr)[j]->id);
				free((a[i]->arr)[j]);
			}
		}
		a[i]->head += head_delta;
		if (tm > a[i]->time)
			a[i]->time = 0;
		else
			a[i]->time -= tm;
	}	
}

int queue_init(int n, Man **arr_man, int len_man, FILE* out)
{
	Queue **arr_queue = arr_queue_alloc(n, len_man);
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
		(arr_queue[ind]->arr)[arr_queue[ind]->tail] = a;
		arr_queue[ind]->tail++;
		arr_queue[ind]->time += a->ts;
	}
	print_queue(arr_queue, n, curr_tm, out);
	free_arr_queue(arr_queue, n);
	return 0;
}
