#ifndef TABLE_H
#define TABLE_H

#include <stdio.h>

typedef struct Item {
	unsigned int key;
	unsigned int par;
	int offset;
	int len;
} Item;

typedef struct Table {
	int msize;
	int csize;
	Item *arr;
	FILE *fd;
} Table;


int insert(Table*, unsigned int, unsigned int, char*);
int delete(Table*, unsigned int);
int find(Table*, unsigned int, Table*);
int load(Table*, char*);
int create(Table*, char*, int);
void print(Table*, FILE*);
void free_table(Table*);

#endif
