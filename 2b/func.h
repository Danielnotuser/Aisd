#ifndef FUNC_H
#define FUNC_H

#include <stdio.h>

#ifdef USE_LIST

typedef struct Man {
	char *id;
	int ta;
	int ts;
	struct Man *next;
} Man;

typedef struct Queue {
	Man *head;
	Man *tail;
} Queue;

#else

typedef struct Man {
	char *id;
	int ta;
	int ts;
} Man;

typedef struct Queue {
	Man **arr;
	int head;
	int tail;
} Queue;

#endif

Queue *queue_alloc();
void free_queue(Queue*);
void print_queue(Queue**, int, int, FILE*);
void push(Queue*, Man*);
void queue_passed_time(Queue**, int*, int, int, int, FILE*);
int queue_short(int*, int);

int queue_init(int, Man**, int, FILE*);
Man **man_init(int*, FILE*, FILE*);
int man_cmp(const Man**, const Man**);

int read_val(int*, FILE*, FILE*);
int read_nat(int*, FILE*, FILE*);

#endif
