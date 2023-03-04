#ifndef QUEUE_H
#define QUEUE_H

#include "data.h"

typedef struct Man {
	char *id;
	int ta;
	int ts;
	struct Man *next;
} Man;

typedef struct Queue {
	Man *tail;
	Man *head;
	int time;
} Queue;

int queue_init(int, Man**, int, FILE*);
Man *man_alloc();
Man **man_init(int*, FILE*, FILE*);
int man_cmp(const Man**, const Man**);

#endif
