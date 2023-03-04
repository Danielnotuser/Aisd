#ifndef LINE_H
#define LINE_H

typedef struct Line {
	int len;
	int *arr;
} Line;

int read_val(int*);
int read_len(int*);
int num_len(int);
int input_matrix(Line**, int, int*);
Line *fix_matrix(Line**, int, int);
void print_matrix(Line*, int);
void free_matrix(Line**, int);

#endif
