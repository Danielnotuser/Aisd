#ifndef TABLE_H
#define TABLE_H

typedef struct Item {
	int busy;
	int rel;
	unsigned int key;
	char *info;
} Item;

typedef struct Table {
	int msize;
	int csize;
	Item *arr;
} Table;


int insert(Table*, unsigned int, char*);
int delete(Table*, unsigned int);
int find(Table*, unsigned int, Table*);
void free_table(Table*);

#endif
