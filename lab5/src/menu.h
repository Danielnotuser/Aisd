#ifndef MENU_H
#define MENU_H

#include "graph.h"

int read_nat(int*);
int read_arr_nat(int**, int);
int read_str(char**);

int menu(const char *opts[], int num_opts);
int dlg_add_vertex(Graph*);
int dlg_add_edge(Graph*);
int dlg_delete_vertex(Graph*);
int dlg_delete_edge(Graph*);
int dlg_change_vertex(Graph*);
int dlg_change_edge(Graph*);
int dlg_find(Graph*);
int dlg_partition(Graph*);
int dlg_detour(Graph*);
int dlg_matr(Graph*);
int dlg_graphviz(Graph*);
int dlg_import(Graph*);
void dlg_free_graph(Graph*);



#endif
