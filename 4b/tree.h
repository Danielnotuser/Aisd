#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Item {
	char *key;
	char *info;
} Item;

typedef struct Node {
	int num_i, num_p;
	Item items[3];
	struct Node *ptr[4];
	struct Node *par;
} Node;

typedef struct Tree {
	Node *root;
} Tree;

int insert(Tree*, char*, char*);
int delete(Tree*, Node*, char*, Item*);
int find(Node*, char*, Item*);
int special_find(Node*, char*, Item*);
void detour(Node*);
void print_node(FILE*, Node*);
void show_recur(Node*, int, char);
void free_recur(Node**);
void free_item(Item*);

#endif
