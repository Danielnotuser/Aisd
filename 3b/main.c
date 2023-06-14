#include <stdlib.h>

#include "menu.h"
#include "table.h"

int main()
{
	Table tbl = {0, 0, NULL, NULL};
	if (dlg_load(&tbl) == 0)
		return 1;
	int var;
	int n = 7;
	const char *opts[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Import from file", "5. Find kids", "6. Print"};
	int (*fptr[])(Table*) = {NULL, dlg_add, dlg_find, dlg_delete, dlg_import, dlg_find_kids, dlg_print};
	while ((var = menu(opts, n)))
	{
		if (!fptr[var](&tbl))
			break;
	}
	dlg_save(&tbl);
	fprintf(stdout, "See you later!\n");
	free_table(&tbl);
	return 0;
}
