#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "menu.h"

int main()
{
	Graph graph;
	graph.verts = NULL;
	graph.vert_num = 0;
	int var, num_opts = 10;
	const char *opts[] = {"0. Quit", "1. Add", "2. Delete", "3. Change data", 
						"4. Find shortest path",  "5. Graph partition", "6. Detour" ,
						"7. Show matrix", "8. Show graph", "9. Import graph"};
	const char *vertedg[] = {"0. Quit", "1. Vertex", "2. Edge"};
	int (*fptr[])(Graph*) = {dlg_find, dlg_partition, dlg_detour, dlg_matr, dlg_graphviz, dlg_import};
	while ((var = menu(opts, num_opts)))
	{
		if (var >= 1 && var <= 3)
		{
			var += var - 1;
			int (*switch_ptr[])(Graph*) = {dlg_add_vertex, dlg_add_edge, dlg_delete_vertex, 
										dlg_delete_edge, dlg_change_vertex, dlg_change_edge};
			int var_switch = menu(vertedg, 3);
			if (!var_switch || switch_ptr[var + var_switch - 2](&graph))
				break;
		}
		else if (fptr[var - 4](&graph))
			break;
	}
	fprintf(stdout, "See you later!\n");
	dlg_free_graph(&graph);
	return 0;
}
