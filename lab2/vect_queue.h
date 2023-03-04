#ifndef VECT_QUEUE
#define VECT_QUEUE

#include "data.h"

typedef struct Man {
	char *id;
	int ta;
	int ts;
} Man;

typedef struct Queue {
	Man **arr;
	int time;
	int head;
	int tail;
} Queue;

int queue_init(int, Man**, int, FILE*);
Man *man_alloc();
Man **man_init(int*, FILE*, FILE*);
int man_cmp(const Man**, const Man**);

#endif
