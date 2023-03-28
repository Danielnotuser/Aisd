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
	if ((tbl->csize == 0 && par != 0) || (!p && par != 0))
		return 2;;
	fseek(tbl->fd, 0, SEEK_END);
	int offset = ftell(tbl->fd);
	Item a = {key, par, offset, strlen(info) + 1};
	(tbl->arr)[tbl->csize] = a;
	fwrite(info, sizeof(char), a.len, tbl->fd); 
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
	/*fseek(tbl->fd, (tbl->arr)[tbl->csize-1].offset, SEEK_SET);
	char *info = (char*) calloc((tbl->arr)[tbl->csize-1].len, sizeof(char);)
	fread(info, sizeof(char), (tbl->arr)[tbl->csize-1].len, tbl->fd);
	fseek(tbl->fd, (tbl->arr)[k].offset, SEEK_SET);
	fwrite
	(tbl->arr)[k] = (tbl->arr)[tbl->csize-1];
	Item a = {0, 0, 0, 0};
	(tbl->arr)[tbl->csize-1] = a;#include <stdio.h>
	tbl->csize -= 1;*/
	return 0;
}

int find(Table *tbl, unsigned int par, Table *res)
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
			new.len = (tbl->arr)[i].len;
			childs[rsize] = new;
			rsize++;
		}
	}
	if (rsize == 0)
		return 1;
	res->msize = rsize;
	res->csize = rsize;
	res->arr = childs;
	return 0;
}

int load(Table *tbl, char *fname)
{
	tbl->fd = fopen(fname, "r+b");
	if (tbl->fd == NULL)
		return 1;
	fread(&tbl->msize, sizeof(int), 1, tbl->fd);
	tbl->arr = (Item*) calloc(tbl->msize, sizeof(Item));
	fread(&tbl->csize, sizeof(int), 1, tbl->fd);
	fread(tbl->arr, sizeof(Item), tbl->csize, tbl->fd);
	return 0;
}

int create(Table *tbl, char *fname, int sz)
{
	tbl->msize = sz;
	tbl->csize = 0;
	tbl->fd = fopen(fname, "w+b");
	if (!tbl->fd)
	{
		tbl->arr = NULL;
		return 1;
	}	
	tbl->arr = (Item*) calloc(tbl->msize, sizeof(Item));
	fwrite(&tbl->msize, sizeof(int), 1, tbl->fd);
	fwrite(&tbl->csize, sizeof(int), 1, tbl->fd);
	fwrite(tbl->arr, sizeof(Item), tbl->msize, tbl->fd);
	return 0;
}

void print(Table *tbl, FILE *out)
{
	if (tbl->csize == 0)
		fprintf(out, "Table is empty.\n");
	for (int i = 0; i < tbl->csize; i++)
	{
		fseek(tbl->fd, (tbl->arr)[i].offset, SEEK_SET);
		char *info = (char*) calloc((tbl->arr)[i].len, sizeof(char));
		fread(info, sizeof(char), (tbl->arr)[i].len, tbl->fd);
		fprintf(out, "par = %u, key = %u, info = %s\n", (tbl->arr)[i].par, (tbl->arr)[i].key, info);
		free(info);
	}
}

void free_table(Table *tbl)
{
	if (tbl->arr)
		free(tbl->arr);
}