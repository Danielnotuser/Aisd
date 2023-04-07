#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "table.h"

int insert(Table *tbl, unsigned int key, unsigned int par, char *info)
{
	if (tbl->csize == tbl->msize)
		return 3;
	int p = 0;
	for (int i = 0; i < tbl->csize; i++)
	{
		if ((tbl->arr)[i].key == key)
			return 1;
		if ((tbl->arr)[i].key == par)
			p = 1;
	}
	if (!p && par != 0)
		return 2;
	char *info_item = strdup(info);
	Item a = {key, par, info_item};
	(tbl->arr)[tbl->csize] = a;
	(tbl->csize)++; 
	return 0;
}

int delete(Table *tbl, unsigned int key)
{
	int k = -1;
	for (int i = 0; i < tbl->csize; i++)
	{
		if ((tbl->arr)[i].par == key)
			return 1;
		if ((tbl->arr)[i].key == key)
			k = i;
	}
	if (k == -1)
		return 2;
	free((tbl->arr)[k].info);
	(tbl->arr)[k] = (tbl->arr)[tbl->csize-1];
	Item a = {0, 0, NULL};
	(tbl->arr)[tbl->csize-1] = a;
	tbl->csize -= 1;
	return 0;
}

int find(Table *tbl, unsigned int key, Item *a)
{
	for (int i = 0; i < tbl->csize; i++)
	{
		if ((tbl->arr)[i].key == key)
		{
			(*a) = (tbl->arr)[i];
			return 0;
		}
	}
	return 1;
}

int find_kids(Table *tbl, unsigned int par, Item **res, int *len)
{
	Item *childs = NULL;
	int rsize = 0;
	for (int i = 0; i < tbl->csize; i++)
	{
		if ((tbl->arr)[i].par == par)
		{
			childs = (Item*) realloc(childs, sizeof(Item) * (rsize + 1));
			if (!childs)
				return 2;
			Item new;
			new.key = (tbl->arr)[i].key;
			new.par = par;
			new.info = strdup((tbl->arr)[i].info);
			if (!new.info)
				return 2;
			childs[rsize] = new;
			rsize++;
		}
	}
	if (rsize == 0)
		return 1;
	(*res) = childs;
	(*len) = rsize;
	return 0;
}


void print(Table *tbl, FILE *out)
{
	if (tbl->csize == 0)
		fprintf(out, "Table is empty.\n");
	for (int i = 0; i < tbl->csize; i++)
	{
		fprintf(out, "par = %u, key = %u, info = %s\n", (tbl->arr)[i].par, (tbl->arr)[i].key, (tbl->arr)[i].info);
	}
}

void free_table(Table *tbl)
{
	if (tbl)
	{
		for (int i = 0; i < tbl->csize; i++)
			free((tbl->arr)[i].info);
		free(tbl->arr);
	}
}
