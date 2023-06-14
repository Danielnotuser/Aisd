#include <stdlib.h>

#include "func.h"

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

void free_arr_queue(Queue **a, int n)
{
	for (int i = 0; i < n; i++)
		free_queue(a[i]);
	free(a);
}

int queue_short(int *time, int n)
{
	int minn = 1000, ind;
	for (int i = 0; i < n; i++)
	{
		if (time[i] == 0)
			return i;
		if (time[i] < minn)
		{
			minn = time[i];
			ind = i;
		}
	}
	return ind;
}

int queue_init(int n, Man **arr_man, int len_man, FILE *out)
{
	Queue **arr_queue = arr_queue_alloc(n);
	if (!arr_queue)
		return 1;
	int *time = (int*) calloc(n, sizeof(int));
	if (!time)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return 1;
	}
	int curr_tm = 0;
	fprintf(out, "\n");
	for (int i = 0; i < len_man; i++)
	{
		if ((i == len_man - 1) || (curr_tm != arr_man[i]->ta))
			print_queue(arr_queue, n, curr_tm, out);
		Man *a = arr_man[i];
		if (!curr_tm) 
			curr_tm = a->ta;
		else if (curr_tm != a->ta)
		{
			queue_passed_time(arr_queue, time, n, curr_tm, a->ta - curr_tm, out);
			curr_tm = a->ta;
		}
		int ind = queue_short(time, n);
		push(arr_queue[ind], a);
		time[ind] += a->ts;
	}
	print_queue(arr_queue, n, curr_tm, out);
	queue_passed_time(arr_queue, time, n, curr_tm, 1000, out);
	free(time);
	free_arr_queue(arr_queue, n);
	return 0;
}
