#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "graph.h"

int menu(const char *opts[], int num_opts)
{
	int var = -1;
	int i, err;
	do
	{
		if (var >= num_opts) printf("Error! Number must be in range of options. Try again...\n");
		for (i = 0; i < num_opts; i++)
			printf("%s\n", opts[i]);
		printf("Choose the option: ");
		err = read_nat(&var);
		if (err)
			var = 0;
	} while (var >= num_opts);
	
	return var;
}

int dlg_add_vertex(Graph* graph)
{
	char *errmsgs[] = {"Vertex added.", "Not unique name.", "Memory allocation error."};
	char *name;
	int port;
	printf("Enter name: ");
	if (read_str(&name)) return 1;
	printf("Enter port: ");
	if (read_nat(&port)) return 1;
	name = (char*) realloc(name, strlen(name) + 1);
	int err = insert_vert(graph, name, port);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	free(name);
	return 0;	
}

int dlg_add_edge(Graph* graph)
{
	char *errmsgs[] = {"Edge added.", "There is no such vertex.", "Edge already exists.", "Loops are prohibited."};
	int port_num = 0, delay, *ports;
	char *from_name, *to_name;
	printf("Enter from which vertex: ");
	if (read_str(&from_name)) return 1;
	printf("Enter to which vertex: ");
	if (read_str(&to_name)) {free(from_name); return 1;}
	while (!port_num)
	{
		printf("Enter number of ports: ");
		if (read_nat(&port_num)) {free(from_name); free(to_name); return 1;}
		if (!port_num) printf("Try again with number greater than zero...\n");
	}
	printf("Enter ports: ");
	if (read_arr_nat(&ports, port_num)) {free(ports); free(from_name); free(to_name); return 1;}
	printf("Enter delay: ");
	if (read_nat(&delay)) {free(ports); free(from_name); free(to_name); return 1;}
	from_name = (char*) realloc(from_name, strlen(from_name) + 1);
	to_name = (char*) realloc(to_name, strlen(to_name) + 1);
	int err = insert_edge(graph, from_name, to_name, &ports, port_num, delay);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	free(from_name);
	free(to_name);
	return 0;	
}


int dlg_delete_vertex(Graph *graph)
{
	char *errmsgs[] = {"Vertex deleted.", "There is no such vertex."};
	char *name;
	printf("Enter name: ");
	if (read_str(&name)) return 1;
	name = (char*) realloc(name, strlen(name) + 1);
	int err = delete_vertex(graph, name);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	free(name);	
	return 0;
}

int dlg_delete_edge(Graph *graph)
{
	char *errmsgs[] = {"Edge deleted.", "There is no such edge."};
	char *from_name, *to_name;
	printf("Enter from which vertex: ");
	if (read_str(&from_name)) return 1;
	printf("Enter to which vertex: ");
	if (read_str(&to_name)) return 1;
	from_name = (char*) realloc(from_name, strlen(from_name) + 1);
	to_name = (char*) realloc(to_name, strlen(to_name) + 1);
	int err = delete_edge(graph, from_name, to_name);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	free(from_name);
	free(to_name);	
	return 0;
}

int dlg_change_vertex(Graph *graph)
{
	char *errmsgs[] = {"Vertex changed.", "There is no such vertex."};
	const char *opts[] = {"0. Quit", "1. Change name", "2. Change port", "3. Change both"};
	char *name, *new_name;
	int port, err, var;
	printf("Enter name: ");
	if (read_str(&name)) return 1;
	if (!(var = menu(opts, 4))) {free(name); return 1;}
	name = (char*) realloc(name, strlen(name) + 1);
	if (var == 1 || var == 3)
	{
		printf("Enter new name: ");
		if (read_str(&new_name)) {free(name); return 1;}
		name = (char*) realloc(name, strlen(name) + 1);	
		if (var == 1) err = change_vertex(graph, name, new_name, -1);
	}
	if (var == 2 || var == 3)
	{
		printf("Enter new port: ");
		if (read_nat(&port)) 
		{
			free(name);
			if (new_name) free(new_name);
			return 1;
		}
		if (var == 2) err = change_vertex(graph, name, NULL, port);
		else err = change_vertex(graph, name, new_name, port);
		
	}
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	free(name);	
	if (var != 2) free(new_name);
	return 0;		
}

int dlg_change_edge(Graph *graph)
{
	char *errmsgs[] = {"Edge changed.", "There is no such edge."};
	const char *opts[] = {"0. Quit", "1. Change ports", "2. Change delay", "3. Change both"};
	char *from_name, *to_name;
	int *ports, err, var, delay, port_num = 0;
	printf("Enter from which vertex: ");
	if (read_str(&from_name)) return 1;
	printf("Enter to which vertex: ");
	if (read_str(&to_name)) return 1;
	if (!(var = menu(opts, 4))) {free(from_name); free(to_name); return 1;}
	from_name = (char*) realloc(from_name, strlen(from_name) + 1);
	to_name = (char*) realloc(to_name, strlen(to_name) + 1);
	if (var == 1 || var == 3)
	{
		while (!port_num)
		{
			printf("Enter new number of ports: ");
			if (read_nat(&port_num)) {free(from_name); free(to_name); return 1;}	
			if (!port_num) printf("Try again with number greater than zero...\n");		
		}
		printf("Enter new ports: ");
		if (read_arr_nat(&ports, port_num)) return 1;
		if (var == 1) err = change_edge(graph, from_name, to_name, port_num, ports, -1);
	}
	if (var == 2 || var == 3)
	{
		printf("Enter new delay: ");
		if (read_nat(&delay)) 
		{
			free(from_name);
			free(to_name);
			if (ports) free(ports);
			return 1;
		}
		if (var == 2) err = change_edge(graph, from_name, to_name, -1, NULL, delay);
		else err = change_edge(graph, from_name, to_name, port_num, ports, delay);
		
	}
	if (err) 
		printf("Error!\n");
	printf("%s\n", errmsgs[err]);
	free(from_name);
	free(to_name);	
	return 0;		
}


int dlg_find(Graph *graph)
{
	char *name1, *name2;
	int ind1 = -1, ind2 = -1;
	printf("Enter name of first vertex: ");
	if (read_str(&name1)) return 1;
	printf("Enter name of second vertex: ");
	if (read_str(&name2)) {free(name2); return 1;}
	for (int i = 0; i < graph->vert_num; i++)
	{
		if (!strcmp(graph->verts[i].name, name1))
			ind1 = i;
		if (!strcmp(graph->verts[i].name, name2))
			ind2 = i;
	}
	free(name1);
	free(name2);
	if (ind1 < 0 || ind2 < 0)
	{
		printf("Error! No such vertex.\n");
		return 0;
	}
	if (ind1 == ind2)
	{
		printf("Error! There is no path from vertex to itself.\n");
		return 0;
	}
	int *path = (int*) calloc(graph->vert_num, sizeof(int));
	int err = dijkstra(graph, ind1, ind2, path);
	if (err)
		printf("Error! There is no way from %s to %s", graph->verts[ind1].name, graph->verts[ind2].name);
	else
	{
		printf("Shortest path from %s to %s: ", graph->verts[ind1].name, graph->verts[ind2].name);
		for (int i = 0; path[i] != ind2; i++)
			printf("%s ", graph->verts[path[i]].name);
		printf("%s\n", graph->verts[ind2].name);	
	}
	free(path);
	return 0;	
}


int dlg_detour(Graph *graph)
{
	if (!graph->vert_num)
	{
		printf("Graph is empty.\n");
		return 0;
	}
	char *name;
	printf("Enter vertex from which DFS will start: ");
	if (read_str(&name)) return 1;
	name = (char*) realloc(name, strlen(name) + 1);
	int err = dfs_init(graph, name);
	if (err)
		printf("Error! No such vertex.\n");
	free(name);
	return 0;
}

int dlg_partition(Graph *graph)
{
	if (!graph->vert_num)
	{
		printf("Graph is empty.\n");
		return 0;
	}
	partition(graph);
	return 0;	
}

int dlg_matr(Graph *graph)
{
	if (!graph->vert_num)
	{
		printf("Graph is empty.\n");
		return 0;
	}
	show_matr(graph);
	return 0;
}

int dlg_graphviz(Graph *graph)
{
	if (!graph->vert_num)
	{
		printf("Graph is empty.\n");
		return 0;
	}
	FILE *fd = fopen("./viz/graph.dot", "w");
	fprintf(fd, "digraph Net_graph {\n");
	fprintf(fd, "rankdir=LR;\n");
	fprintf(fd, "edge[dir=none];\n");
	print_graph(fd, graph);
	fprintf(fd, "}");
	fclose(fd);
	system("dot -Tpng ./viz/graph.dot -o ./viz/graph.png");
	system("catimg ./viz/graph.png");
	return 0;
}

int dlg_import(Graph *graph)
{
	char *fname;
	printf("Enter file name: ");
	if (read_str(&fname)) return 1;
	FILE *f = fopen(fname, "r");
	if (f == NULL)
	{
		printf("Error! Can't open file \"%s\"\n", fname);
		free(fname);
		return 1;
	}
	free_graph(graph);
	int len;
	graph->vert_num = 0;
	graph->verts = NULL;
	fscanf(f, "%d%*c", &len);
	for (int i = 0; i < len; i++)
	{
		char *name = (char*) calloc(40, sizeof(char));
		int pos = ftell(f);
		fscanf(f, "%[^\n]", name);
		pos = ftell(f) - pos;
		name[pos] = '\0';
		name = (char*) realloc(name, (pos + 1) * sizeof(char));
		int port;
		fscanf(f, "%d%*c", &port);
		insert_vert(graph, name, port);
		free(name);
	}
	int size;
	fscanf(f, "%d%*c", &size);
	for (int i = 0; i < size; i++)
	{
		char *name1 = (char*) calloc(40, sizeof(char));
		int pos = ftell(f);
		fscanf(f, "%[^\n]%*c", name1);
		pos = ftell(f) - pos;
		name1[pos] = '\0';
		name1 = (char*) realloc(name1, (pos + 1) * sizeof(char));
		char *name2 = (char*) calloc(40, sizeof(char));
		pos = ftell(f);
		fscanf(f, "%[^\n]", name2);
		pos = ftell(f) - pos;
		name2[pos] = '\0';
		name2 = (char*) realloc(name2, (pos + 1) * sizeof(char));
		int port_num, delay;
		fscanf(f, "%d%*c", &port_num);
		int *ports = (int*) calloc(port_num, sizeof(int));
		for (int j = 0; j < port_num; j++)
			fscanf(f, "%d%*c", &ports[j]);
		fscanf(f, "%d%*c", &delay);
		insert_edge(graph, name1, name2, &ports, port_num, delay);
		free(name1);
		free(name2);
	}
	fclose(f);
	free(fname);
	return 0;
}

void dlg_free_graph(Graph *graph)
{
	if (graph->vert_num)
		free_graph(graph);
}

