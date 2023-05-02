#include <stdlib.h>

#include "tree.h"
#include "menu.h"

int main()
{
	Tree tree;
	tree.root = NULL;
	int var, num_opts = 8;
	const char *opts[] = {"0. Quit", "1. Add", "2. Delete", "3. Find", 
			"4. Special find",  "5. Import", "6. Detour" ,"7. Show tree", "8. Functiom timing"};
	int (*fptr[])(Tree*) = {dlg_add, dlg_delete, dlg_find, dlg_special_find, dlg_import, 
									dlg_detour, dlg_show};
	while ((var = menu(opts, num_opts)))
	{
		if (!var || fptr[var - 1](&tree))
			break;
	}
	fprintf(stdout, "See you later!\n");
	free_tree(&tree);
	return 0;
}
