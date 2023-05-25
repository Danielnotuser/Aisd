#ifndef GRAPH_H
#define GRAPH_H

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

typedef struct Graph {
	int vert_num;
	struct Vertex *verts;
} Graph;

typedef struct Vertex {
	char *name;
	int port;
	int edge_num;
	struct Edge **edges;
} Vertex;

typedef struct Edge {
	int from;
	int to;
	int port_num;
	int *ports;
	int delay;
} Edge;

int insert_vert(Graph*, char*, int);
int insert_edge(Graph*, char*, char*, int**, int, int);
int delete_vertex(Graph*, char*);
int delete_edge(Graph*, char*, char*);
int change_vertex(Graph*, char*, char*, int);
int change_edge(Graph*, char*, char*, int, int*, int);
void free_graph(Graph*);
int dfs_init(Graph*, char*);
int dijkstra(Graph*, int, int, int*);
void partition(Graph*);
void show_matr(Graph*);
void print_graph(FILE*, Graph*);

#endif 
