#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Item {
	char **arr;
	int n;
} Item;

typedef struct Node {
	char *key;
	Item info;
	struct Node *left;
	struct Node *right;
	struct Node *par;
	struct Node *next;
} Node;

typedef struct Tree {
	Node *root;
} Tree;

int insert(Tree*, char*, char*);
int delete(Tree*, char*, int);
int find(Tree*, char*, int, Node**);
int special_find(Tree*, char*, int, Node**);
void detour(Tree*);
void print_node(FILE*, Node*);
void show_recur(Node*, int, char);
void free_recur(Node**);

#endif
