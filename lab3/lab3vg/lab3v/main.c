#include <stdlib.h>

#include "menu.h"
#include "table.h"

int main()
{
	Table tbl;
	int size;
	printf("Write size of table: ");
	int err = read_nat(&size);
	if (err)
		return 1;
	tbl.arr = (Item*) calloc(size, sizeof(Item));
	tbl.msize = size;
	if (!tbl.arr)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return 2;
	}
	tbl.csize = 0;
	for (int i = 0; i < tbl.msize; i++)
		(tbl.arr)[i].busy = 0;
	int var;
	int n = 6;
	const char *opts[] = {"0. Quit", "1. Add", "2. Find", "3. Delete", "4. Import from file", "5. Print"};
	int (*fptr[])(Table*) = {NULL, dlg_add, dlg_find, dlg_delete, dlg_import, print};
	while ((var = menu(opts, n)))
	{
		if (!fptr[var](&tbl))
			break;
	}
	fprintf(stdout, "See you later!\n");
	free_table(&tbl);
	return 0;
}
