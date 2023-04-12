#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "menu.h"
#include "table.h"

#define STEP 1

unsigned int hash(unsigned int key)
{
	return key;	
}

int find_rel(Table *tbl, unsigned int key) // Возможно использование find() вместо find_rel()?
{
	int rel = 0, h;
	int M = tbl->msize;
	int h_0 = hash(key);
	for (int n = 0, p = 0; n < tbl->msize; n++, p += STEP)
	{
		h = (h_0 + p) % M;
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
	unsigned int h, h0 = hash(key);
	int M = tbl->msize;
	int len = strlen(info) + 1;
	for (int n = 0, p = 0; n < tbl->msize; n++, p += STEP)
	{
		h = (h0 + p) % M;
		if ((tbl->arr)[h].busy != 1)
		{
			(tbl->arr)[h].busy = 1;
			(tbl->arr)[h].rel = find_rel(tbl, key);
			(tbl->arr)[h].key = key;
			(tbl->arr)[h].len = len;
			for (unsigned int i = tbl->msize - 1; i > h; i--)
			{
				int offset = (tbl->arr)[i].offset;
				(tbl->arr)[i].offset += len - 1;
				if ((tbl->arr)[i].busy == 1)
				{
					fseek(tbl->fd, offset, SEEK_SET);
					char *info_i = (char*) calloc((tbl->arr)[i].len, sizeof(char));
					if (!info_i) return 2;
					fread(info_i, sizeof(char), (tbl->arr)[i].len, tbl->fd);
					fseek(tbl->fd, offset + len - 1, SEEK_SET);
					fwrite(info_i, sizeof(char), (tbl->arr)[i].len, tbl->fd);	
					free(info_i);
				}
				
			}
			fseek(tbl->fd, (tbl->arr)[h].offset, SEEK_SET);
			fwrite(info, sizeof(char), (tbl->arr)[h].len, tbl->fd);
			break;
		}
	}
	(tbl->csize)++; 
	return 0;
}


int delete(Table *tbl, unsigned int key)
{
	unsigned int h, h_0 = hash(key);
	int M = tbl->msize, len, file_length;
	if ((tbl->arr)[tbl->msize - 1].busy == 1)
		file_length = (tbl->arr)[tbl->msize - 1].offset + (tbl->arr)[tbl->msize - 1].len;
	else 
		file_length = (tbl->arr)[tbl->msize - 1].offset + 1;
	for (int n = 0, p = 0; n < tbl->msize; n++, p += STEP)
	{
		h = (h_0 + p) % M;
		if ((tbl->arr)[h].busy == 0)
			return 1;
		if ((tbl->arr)[h].busy == 1 && (tbl->arr)[h].key == key)
		{
			(tbl->arr)[h].busy = -1;
			len = (tbl->arr)[h].len;
			for (int i = h + 1; i < tbl->msize; i++)
			{
				int offset = (tbl->arr)[i].offset;
				(tbl->arr)[i].offset -= (len - 1);
				if ((tbl->arr)[i].busy == 1)
				{
					fseek(tbl->fd, offset, SEEK_SET);
					char *info_i = (char*) calloc((tbl->arr)[i].len, sizeof(char));
					if (!info_i) return 2;
					fread(info_i, sizeof(char), (tbl->arr)[i].len, tbl->fd);
					fseek(tbl->fd, offset - len + 1, SEEK_SET);
					fwrite(info_i, sizeof(char), (tbl->arr)[i].len, tbl->fd);	
					free(info_i);
				}		
			}
			break;
		}
	}
	int fd = fileno(tbl->fd);
	fflush(tbl->fd);
	ftruncate(fd, file_length - (len * sizeof(char)));
	(tbl->csize)--;
	return 0;
}

int find(Table *tbl, unsigned int key, Table *res)
{
	Item *rels = (Item*) calloc(tbl->msize, sizeof(Item));
	Table r;
	if (!rels)
		return 2;
	int h, len = 0;
	int M = tbl->msize;
	int h_0 = hash(key);
	for (int n = 0, p = 0; n < tbl->msize; n++, p += STEP)
	{
		h = (h_0 + p) % M;
		if ((tbl->arr)[h].busy == 0)
			break;
		if ((tbl->arr)[h].busy == 1 && (tbl->arr)[h].key == key)
		{
			Item a = {(tbl->arr)[h].busy, (tbl->arr)[h].rel, (tbl->arr)[h].key, (tbl->arr)[h].offset, (tbl->arr)[h].len};
			rels[len] = a;
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
	r.fd = tbl->fd;
	(*res) = r;
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
	fread(tbl->arr, sizeof(Item), tbl->msize, tbl->fd);
	return 0;
}

int create(Table *tbl, char *fname, int sz)
{
	tbl->msize = sz;
	tbl->csize = 0;
	if (fname) 
		tbl->fd = fopen(fname, "w+b");
	if (!tbl->fd)
	{
		tbl->arr = NULL;
		return 1;
	}	
	tbl->arr = (Item*) calloc(tbl->msize, sizeof(Item));
	fwrite(&(tbl->msize), sizeof(int), 1, tbl->fd);
	fwrite(&(tbl->csize), sizeof(int), 1, tbl->fd);
	fwrite(tbl->arr, sizeof(Item), tbl->msize, tbl->fd);
	(tbl->arr)[0].offset = ftell(tbl->fd); 
	(tbl->arr)[0].busy = 0;
	for (int i = 1; i < tbl->msize; i++)
	{
		(tbl->arr)[i].busy = 0;
		(tbl->arr)[i].offset = (tbl->arr)[i - 1].offset + 1;
	}
	return 0;
}

int print(Table *tbl)
{
	if (tbl->csize == 0)
		printf("Table is empty.\n");
	for (int i = 0; i < tbl->msize; i++)
	{
		if ((tbl->arr)[i].busy == 1)
		{
			fseek(tbl->fd, (tbl->arr)[i].offset, SEEK_SET);
			char *info = (char*) calloc((tbl->arr)[i].len, sizeof(char));
			fread(info, sizeof(char), (tbl->arr)[i].len, tbl->fd);
			printf("busy = %d, rel = %d, key = %u, info = %s\n", (tbl->arr)[i].busy, (tbl->arr)[i].rel, (tbl->arr)[i].key, info);
			free(info);	
		}
	}
	return 1;
}

void free_table(Table *tbl)
{
	if (tbl)
		free(tbl->arr);
}
