#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

#define INF 10000000

static void shift_edge(Vertex *v, int dest)
{
	for (int i = 0; i < v->edge_num; i++)
	{
		if (v->edges[i]->from == dest || v->edges[i]->to == dest)
		{
			free(v->edges[i]->ports);
			free(v->edges[i]);
			for (int j = i; j < v->edge_num - 1; j++)
				v->edges[j] = v->edges[j + 1];
			v->edge_num--;
			break;
		}
	}
}

static void shift_vertex(Graph *graph, int ind)
{
	if (graph->verts[ind].edges) free(graph->verts[ind].edges);
	free(graph->verts[ind].name);
	for (int i = ind; i < graph->vert_num; i++)
	{
		if (i > ind)
		{
			for (int j = 0; j < graph->verts[i].edge_num; j++)
			{
				if (i == graph->verts[i].edges[j]->from)
					graph->verts[i].edges[j]->from--;
				else
					graph->verts[i].edges[j]->to--;
			}
		}
		if (i != graph->vert_num - 1) graph->verts[i] = graph->verts[i + 1];
	}
	graph->vert_num--;	
}

static void free_vertex(Graph *graph, int ind)
{
	for (int i = 0; i < graph->verts[ind].edge_num; i++)
	{
		int to = graph->verts[ind].edges[i]->to;
		int from = graph->verts[ind].edges[i]->from;
		if (to == ind)
		{
			to = graph->verts[ind].edges[i]->from;
			from = graph->verts[ind].edges[i]->to;	
		} 
		shift_edge(&graph->verts[to], from);
	}	
	shift_vertex(graph, ind);
}

void free_graph(Graph *graph)
{
	while (graph->vert_num)
		free_vertex(graph, 0);	
	if (graph->verts) free(graph->verts);
}

int insert_vert(Graph *graph, char *name, int port)
{
	for (int i = 0; i < graph->vert_num; i++)
		if (!strcmp(graph->verts[i].name, name))
			return 1;
	graph->vert_num++;
	Vertex new;
	new.name = strdup(name);
	if (!new.name)
		return 2;
	new.port = port;
	new.edge_num = 0;
	new.edges = NULL;
	graph->verts = (Vertex*) realloc(graph->verts, graph->vert_num * sizeof(Vertex));
	graph->verts[graph->vert_num - 1] = new;
	return 0;
}

int insert_edge(Graph *graph, char *from_name, char *to_name, int **ports, int port_num, int delay)
{
	Edge *new = (Edge*) calloc(1, sizeof(Edge));
	new->from = -1;
	new->to = -1;
	for (int i = 0; i < graph->vert_num; i++)
	{
		if (!strcmp(graph->verts[i].name, from_name))
			new->from = i;
		else if (!strcmp(graph->verts[i].name, to_name))
			new->to = i;
		if (new->from > 0 && new->to > 0)
			break;
	}
	if (new->from < 0 || new->to < 0)
		return 1;
	if (new->from == new->to)
		return 3;
	new->port_num = port_num;
	new->ports = (*ports);
	new->delay = delay;
	for (int i = 0; i < graph->verts[new->from].edge_num; i++)
		if (graph->verts[new->from].edges[i]->to == new->to || graph->verts[new->from].edges[i]->from == new->to)
			return 2;
	graph->verts[new->from].edge_num++;
	graph->verts[new->from].edges = (Edge**) realloc(graph->verts[new->from].edges, graph->verts[new->from].edge_num * sizeof(Edge*));
	graph->verts[new->from].edges[graph->verts[new->from].edge_num - 1] = new;
	graph->verts[new->to].edge_num++;
	graph->verts[new->to].edges = (Edge**) realloc(graph->verts[new->to].edges, graph->verts[new->to].edge_num * sizeof(Edge*));
	graph->verts[new->to].edges[graph->verts[new->to].edge_num - 1] = new;
	return 0;
}

int delete_vertex(Graph *graph, char *name)
{
	for (int i = 0; i < graph->vert_num; i++)
	{
		if (!strcmp(graph->verts[i].name, name))
		{
			free_vertex(graph, i);
			return 0;
		}
	}
	return 1;
}

int delete_edge(Graph *graph, char *from_name, char *to_name)
{
	int to, j;
	for (int i = 0; i < graph->vert_num; i++)
	{
		to = -1;
		if (!strcmp(graph->verts[i].name, from_name))
		{
			for (j = 0; j < graph->verts[i].edge_num; j++)
			{
				if (!strcmp(graph->verts[graph->verts[i].edges[j]->to].name, to_name))
				{
					to = graph->verts[i].edges[j]->to;
					break;
				}
			}
			if (to < 0) return 1;
			for (int k = j; k < graph->verts[i].edge_num - 1; k++)
				graph->verts[i].edges[k] = graph->verts[i].edges[k + 1];
			graph->verts[i].edge_num--;
			shift_edge(&graph->verts[to], i);
			return 0;
		}
		else if (!strcmp(graph->verts[i].name, to_name))
		{
			for (j = 0; j < graph->verts[i].edge_num; j++)
			{
				if (!strcmp(graph->verts[graph->verts[i].edges[j]->from].name, from_name))
				{
					to = graph->verts[i].edges[j]->from;
					break;
				}
			}
			if (to < 0) return 1;
			for (int k = j; k < graph->verts[i].edge_num - 1; k++)
				graph->verts[i].edges[k] = graph->verts[i].edges[k + 1];
			graph->verts[i].edge_num--;			
			shift_edge(&graph->verts[to], i);	
			return 0;		
		}
	}
	return 1;
}

int change_vertex(Graph *graph, char *name, char *new_name, int port)
{
	for (int i = 0; i < graph->vert_num; i++)
	{
		if (!strcmp(graph->verts[i].name, name))
		{
			if (new_name)	
			{
				free(graph->verts[i].name);
				graph->verts[i].name = strdup(new_name);
			}
			if (port > 0)
				graph->verts[i].port = port;
			return 0;
		}
	}
	return 1;	
}

int change_edge(Graph *graph, char *from_name, char *to_name, int port_num, int *ports, int delay)
{
	for (int i = 0; i < graph->vert_num; i++)
	{
		if (!strcmp(graph->verts[i].name, from_name) || !strcmp(graph->verts[i].name, to_name))
		{
			for (int j = 0; j < graph->verts[i].edge_num; j++)
			{
				Edge *edge = graph->verts[i].edges[j];
				if ((edge->to == i && !strcmp(graph->verts[edge->from].name, from_name)) || (edge->from == i && !strcmp(graph->verts[edge->to].name, to_name)))
				{
					if (ports)
					{
						free(edge->ports);
						edge->port_num = port_num;
						edge->ports = ports;
					}
					if (delay > 0)
						edge->delay = delay;
					return 0;
				}
			}
		}
	}	
	return 1;
}

static int reachable(int *ports, int port_num, int port)
{
	if (!ports) return 0;
	for (int i = 0; i < port_num; i++)
		if (ports[i] == port)
			return 1;
	return 0;	
}

static void dfs(Graph *graph, int *visited, int ind, int port)
{
	visited[ind] = 1;
	for (int i = 0; i < graph->verts[ind].edge_num; i++)
	{
		if (graph->verts[ind].edges[i]->from == ind && !visited[graph->verts[ind].edges[i]->to] &&
		reachable(graph->verts[ind].edges[i]->ports, graph->verts[ind].edges[i]->port_num, port))
		{
			if (graph->verts[graph->verts[ind].edges[i]->to].port == port) printf("%s ", graph->verts[graph->verts[ind].edges[i]->to].name);
			dfs(graph, visited, graph->verts[ind].edges[i]->to, port);
		}
		else if (graph->verts[ind].edges[i]->to == ind && !visited[graph->verts[ind].edges[i]->from] &&
		reachable(graph->verts[ind].edges[i]->ports, graph->verts[ind].edges[i]->port_num, port))
		{	
			if (graph->verts[graph->verts[ind].edges[i]->from].port == port) printf("%s ", graph->verts[graph->verts[ind].edges[i]->from].name);
			dfs(graph, visited, graph->verts[ind].edges[i]->from, port);
		}			
	}
}

int dfs_init(Graph *graph, char *name, int port)
{
	int *visited = (int*) calloc(graph->vert_num, sizeof(int));
	int ind = -1;
	for (int i = 0; i < graph->vert_num; i++)
	{
		if (!strcmp(graph->verts[i].name, name))
			ind = i;	
		visited[i] = 0;
	}
	if (ind < 0)
	{
		free(visited);
		return 1;
	}
	printf("Reachable computers with this port: ");
	dfs(graph, visited, ind, port);
	printf("\n");
	free(visited);
	return 0;
}

static void dfs_part(Graph *graph, int *visited, int ind, int *index, int *ind_num)
{
	visited[ind] = 1;
	index[*ind_num] = ind;
	(*ind_num)++;
	for (int i = 0; i < graph->verts[ind].edge_num; i++)
	{
		if (graph->verts[ind].edges[i]->from == ind && !visited[graph->verts[ind].edges[i]->to])
			dfs_part(graph, visited, graph->verts[ind].edges[i]->to, index, ind_num);
		else if (graph->verts[ind].edges[i]->to == ind && !visited[graph->verts[ind].edges[i]->from])
			dfs_part(graph, visited, graph->verts[ind].edges[i]->from, index, ind_num);
	}	
}

void print_part(Graph *graph, int *index, int ind_num)
{
	FILE *fd = fopen("./viz/graph.dot", "w");
	fprintf(fd, "digraph Net_graph {\n");
	fprintf(fd, "rankdir=LR;\n");
	fprintf(fd, "edge[dir=none];\n");
	for (int i = 0; i < graph->vert_num; i++)
	{
		if (reachable(index, ind_num, i))
		{
			fprintf(fd, "\"%s\";\n", graph->verts[i].name);
			for (int j = 0; j < graph->verts[i].edge_num; j++)
			{
				if (i != graph->verts[i].edges[j]->to && reachable(index, ind_num, graph->verts[i].edges[j]->to))
					fprintf(fd, "\"%s\"->\"%s\"[label=\"%d\"]\n", graph->verts[i].name, graph->verts[graph->verts[i].edges[j]->to].name, graph->verts[i].edges[j]->delay);
			}			
		}
	}	
	fprintf(fd, "}");
	fclose(fd);
	system("dot -Tpng ./viz/graph.dot -o ./viz/graph.png");
	system("catimg ./viz/graph.png");
}

void partition(Graph *graph)
{
	int *visited = (int*) calloc(graph->vert_num, sizeof(int));
	int *index;
	int ind_num = 0;
	for (int i = 0; i < graph->vert_num; i++)	
		visited[i] = 0;
	for (int i = 0; i < graph->vert_num; i++)
	{
		if (!visited[i])
		{
			index = (int*) calloc(graph->vert_num, sizeof(int));
			dfs_part(graph, visited, i, index, &ind_num);
			print_part(graph, index, ind_num);
			free(index);
			ind_num = 0;
		}
	}
	free(visited);
}

int dijkstra(Graph *graph, int ind_from, int ind_to, int *path)
{
	int len = graph->vert_num, min, min_ind, dest_port = graph->verts[ind_to].port;
	int *visited = (int*) calloc(len, sizeof(int));
	int *dist = (int*) calloc(len, sizeof(int));
	int *prev = (int*) calloc(len, sizeof(int));
	for (int i = 0; i < len; i++)
	{
		visited[i] = 0;
		dist[i] = INF;
		prev[i] = -1;
	}	
	dist[ind_from] = 0;
	do
	{
		min = INF;
		min_ind = 0;
		for (int i = 0; i < len; i++)
			if (!visited[i] && dist[i] < min)
			{
				min = dist[i];
				min_ind = i;
			}
		if (min == INF)
			break;
		if (min_ind == ind_to)
			break;
		for (int j = 0; j < graph->verts[min_ind].edge_num; j++)
		{
			int new_dist = dist[min_ind] + graph->verts[min_ind].edges[j]->delay;
			if (reachable(graph->verts[min_ind].edges[j]->ports, graph->verts[min_ind].edges[j]->port_num, dest_port) &&
				min_ind != graph->verts[min_ind].edges[j]->to && new_dist < dist[graph->verts[min_ind].edges[j]->to])
			{
				dist[graph->verts[min_ind].edges[j]->to] = new_dist;
				prev[graph->verts[min_ind].edges[j]->to] = min_ind;
			}
			else if (reachable(graph->verts[min_ind].edges[j]->ports, graph->verts[min_ind].edges[j]->port_num, dest_port) &&
				min_ind == graph->verts[min_ind].edges[j]->to && new_dist < dist[graph->verts[min_ind].edges[j]->from])
			{
				dist[graph->verts[min_ind].edges[j]->from] = new_dist;
				prev[graph->verts[min_ind].edges[j]->from] = min_ind;				
			}				

		}
		visited[min_ind] = 1;
	} while(min != INF);
	if (dist[ind_to] == INF)
	{
		free(visited);
		free(dist);
		free(prev);
		return 1;
	} 
	int i = ind_to, path_ind = len - 1;
	while (i != ind_from)
	{
		path[path_ind] = i;
		i = prev[i];
		path_ind--;
	}
	path[path_ind] = ind_from;
	for (int i = 0; i < len - path_ind; i++)
		path[i] = path[i + path_ind];
	free(visited);
	free(dist);
	free(prev);
	return 0;
}

void show_matr(Graph *graph)
{
	for (int i = 0; i < graph->vert_num; i++)
		printf("%s ", graph->verts[i].name);
	printf("\n");
	int matr[graph->vert_num][graph->vert_num];
	for (int i = 0; i < graph->vert_num; i++)
	{
		for (int k = 0; k < graph->vert_num; k++)
			matr[i][k] = 0;
		for (int j = 0; j < graph->verts[i].edge_num; j++)
		{
			if (i == graph->verts[i].edges[j]->from)
				matr[i][graph->verts[i].edges[j]->to] = 1;
			else
				matr[i][graph->verts[i].edges[j]->from] = 1;
		}
	}
	for (int i = 0; i < graph->vert_num; i++)
	{
		for (int k = 0; k < graph->vert_num; k++)
			printf("%d ", matr[i][k]);
		printf("\n");
	}
}

void print_graph(FILE *fd, Graph *graph)
{
	for (int i = 0; i < graph->vert_num; i++)
	{
		fprintf(fd, "\"%s\";\n", graph->verts[i].name);
		for (int j = 0; j < graph->verts[i].edge_num; j++)
		{
			if (i != graph->verts[i].edges[j]->to)
				fprintf(fd, "\"%s\"->\"%s\"[label=\"%d\"]\n", graph->verts[i].name, graph->verts[graph->verts[i].edges[j]->to].name, graph->verts[i].edges[j]->delay);
		}
	}
}
