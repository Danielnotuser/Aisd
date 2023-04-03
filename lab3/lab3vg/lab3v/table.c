#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "table.h"

#define STEP 1

unsigned int hash(unsigned int key, int p) 
{
	return (key + p);
}

int find_rel(Table *tbl, unsigned int key) // Возможно использование find() вместо find_rel()?
{
	int rel = 0, h;
	int M = tbl->msize;
	for (int n = 0, p = 0; n < tbl->msize; n++, p += STEP)
	{
		h = hash(key, p) % M;
		if ((tbl->arr)[h].busy == 0)
			break;
		if ((tbl->arr)[h].key == key)
			rel = (tbl->arr)[h].rel + 1;
	}	
	return rel;
}

int insert(Table *tbl, unsigned int key, char *info)
{
	if (tbl->csize == tbl->msize)
		return 1;
	char *info_item = strdup(info);
	unsigned int h;
	int busy = 1, rel = 0;
	int M = tbl->msize;
	Item a = {busy, rel, key, info_item};
	for (int n = 0, p = 0; n < tbl->msize; n++, p += STEP)
	{
		h = hash(key, p) % M;
		if ((tbl->arr)[h].busy != 1)
		{
			a.rel = find_rel(tbl, key);
			(tbl->arr)[h] = a;
			break;
		}
	} 
	(tbl->csize)++; 
	return 0;
}

int delete(Table *tbl, unsigned int key)
{
	int h;
	int M = tbl->msize;
	for (int n = 0, p = 0; n < tbl->msize; n++, p += STEP)
	{
		h = hash(key, p) % M;
		if ((tbl->arr)[h].busy == 0)
			return 1;
		if ((tbl->arr)[h].busy == 1 && (tbl->arr)[h].key == key)
		{
			(tbl->arr)[h].busy = -1;
			free((tbl->arr)[h].info);	
			return 0;
		}
	}
}

int find(Table *tbl, unsigned int key, Table *res)
{
	Item *rels = (Item*) calloc(tbl->msize, sizeof(Item));
	Table r;
	if (!rels)
		return 2;
	int h, len = 0;
	int M = tbl->msize;
	for (int n = 0, p = 0; n < tbl->msize; n++, p += STEP)
	{
		h = hash(key, p) % M;
		if ((tbl->arr)[h].busy == 0)
			break;
		if ((tbl->arr)[h].busy == 1 && (tbl->arr)[h].key == key)
		{
			rels[len].busy = (tbl->arr)[h].busy;
			rels[len].rel = (tbl->arr)[h].rel;
			rels[len].key = (tbl->arr)[h].key;
			rels[len].info = strdup((tbl->arr)[h].info);
			len++;
		}
	}
	rels = (Item*) realloc(rels, len * sizeof(Item));
	if (len == 0)
	{
		res = NULL;
		return 1;
	}
	r.csize = len;
	r.msize = len;
	r.arr = rels;
	(*res) = r;
	return 0;
}


int print(Table *tbl)
{
	if (tbl->csize == 0)
		printf("Table is empty.\n");
	for (int i = 0; i < tbl->csize; i++)
	{
		if ((tbl->arr)[i].busy == 1)
			printf("busy = %d, rel = %d, key = %u, info = %s\n", (tbl->arr)[i].busy, (tbl->arr)[i].rel, (tbl->arr)[i].key, (tbl->arr)[i].info);
	}
	return 1;
}

void free_table(Table *tbl)
{
	if (tbl)
	{
		for (int i = 0; i < tbl->msize; i++)
		{
			if ((tbl->arr)[i].busy == 1)
				free((tbl->arr)[i].info);	
		}
		free(tbl->arr);
	}
}
