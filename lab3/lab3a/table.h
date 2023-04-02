#ifndef TABLE_H
#define TABLE_H

typedef struct Item {
	unsigned int key;
	unsigned int par;
	char *info;
} Item;

typedef struct Table {
	int msize;
	int csize;
	Item *arr;
} Table;


int insert(Table*, unsigned int, unsigned int, char*);
int delete(Table*, unsigned int);
int find(Table*, unsigned int, Item**, int*);
void free_table(Table*);

#endif
