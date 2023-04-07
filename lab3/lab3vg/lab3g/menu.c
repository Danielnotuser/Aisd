#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "menu.h"
#include "table.h"

int dlg_load(Table* tbl)
{
	int sz;
	char *fname;
	printf("Enter file name: ");
	int err = read_str(&fname);
	if (err)
	{
		printf("\nInput has been interrupted.\n");
		free(fname);
		return 0;
	}
	fname = (char*) realloc(fname, (strlen(fname)+1) * sizeof(char));
	if (strcmp(strrchr(fname, '.'), ".bin"))
	{
		printf("Error! Wrong file name.");
		return 1;
	}
	if (load(tbl, fname))
	{
		printf("Enter size of table: ");
		err = read_nat(&sz);
		if (err)
		{
			free(fname);
			return 0;
		}
		create(tbl, fname, sz);
	}
	free(fname);
	return 1;
}

int menu(const char *opts[], int n)
{
	int var = -1;
	int i, err;
	do
	{
		if (var >= n) printf("Error! Number must be in range of options. Try again...");
		for (i = 0; i < n; i++)
			printf("%s\n", opts[i]);
		printf("Choose the option: ");
		err = read_nat(&var);
		if (err)
			var = 0;
	} while (var >= n);
	
	return var;
}

int dlg_add(Table* tbl)
{
	const char *errmsgs[] = {"Item added.", "Duplicate key.", 
							"Parent key doesn't exist.", "Table overflow.", 
							"Memory allocation error."};
	int inp;
	unsigned int key, par;
	printf("Enter parent key: ");
	int err = read_nat(&inp);
	if (err)
	{
		printf("\nInput has been interrupted.\n");
		return 0;
	}
	par = inp;
	printf("Enter key: ");
	err = read_nat(&inp);
	if (err)
	{
		printf("\nInput has been interrupted.\n");
		return 0;
	}
	key = inp;
	char *info;
	printf("Enter info: ");
	err = read_str(&info);
	if (err)
	{
		printf("\nInput has been interrupted.\n");
		return 0;
	}
	info = (char*) realloc(info, strlen(info) + 1);
	err = insert(tbl, key, par, info);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	free(info);
	return 1;
}

int dlg_find(Table *tbl)
{
	const char *errmsgs[] = {"Ok", "This item is not a parent key.", 
							"Memory allocation error."};
	int inp;
	unsigned int par;
	printf("Enter key: ");
	int err = read_nat(&inp);
	if (err)
	{
		printf("\nInput has been interrupted.\n");
		return 0;
	}
	par = inp;
	Item *res = NULL;
	int len = 0;
	err = find(tbl, par, &res, &len);
	if (!err)
	{
		printf("Found table:\n");
		for (int i = 0; i < len; i++)
		{
			fseek(tbl->fd, res[i].offset, SEEK_SET);
			char *info = (char*) calloc((tbl->arr)[i].len, sizeof(char));
			fread(info, sizeof(char), res[i].len, tbl->fd);
			printf("par = %u, key = %u, info = %s\n", res[i].par, res[i].key, info);
			free(info);
		}
		if (res) free(res);
	}
	else
		printf("Error! %s\n", errmsgs[err]);
	return 1;
}

int dlg_delete(Table *tbl)
{
	const char *errmsgs[] = {"Item removed.", "This item is a parent key.",
							"There is no item with this key."};
	int inp;
	unsigned int key;
	printf("Enter key: ");
	int err = read_nat(&inp);
	if (err)
	{
		printf("\nInput has been interrupted.\n");
		return 0;	
	}
	key = inp;
	err = delete(tbl, key);
	if (err)
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	return 1;
}

int dlg_import(Table *tbl_curr)
{
	char *inp = (char*) calloc(40, sizeof(char));
	printf("Enter file name: ");
	scanf("%s", inp);
	inp = (char*) realloc(inp, strlen(inp) + 1);
	if (strcmp(strrchr(inp, '.'), ".txt"))
	{
		printf("Error! Wrong file name.\n");
		return 1;
	}
	FILE *f = fopen(inp, "r");
	if (f == NULL)
	{
		printf("Error! Can't open file \"%s\"", inp);
		return 1;
	}
	int size;
	unsigned int par, key;
	int err = fscanf(f, "%d%*c", &size);
	if (err == 1)
	{
		char *info;
		int pos, add_err, i = 0;
		Table tbl;
		tbl.msize = size;
		tbl.arr = (Item*) calloc(size, sizeof(Item));
		tbl.csize = 0;
		tbl.fd = tbl_curr->fd;
		int fd = fileno(tbl.fd);
		fflush(tbl.fd);
		ftruncate(fd, 0);
		if (!tbl.fd) printf("error\n");
		fseek(tbl.fd, 0, SEEK_SET);
		fwrite(&(tbl.msize), sizeof(int), 1, tbl.fd);
		fseek(tbl.fd, sizeof(int), SEEK_SET);
		fwrite(&(tbl.csize), sizeof(int), 1, tbl.fd);
		fseek(tbl.fd, sizeof(int) * 2, SEEK_SET);
		fwrite(tbl.arr, sizeof(Item), size, tbl.fd);
		do
		{
			err = fscanf(f, "%u%*c", &par);
			if (err != 1)
			{
				fscanf(f, "%*[^\n]%*c");
				continue;
			}
			err = fscanf(f, "%u%*c%*c", &key);
			if (err != 1)
			{
				fscanf(f, "%*[^\n]%*c");
				continue;
			}
			info = (char*) calloc(40, sizeof(char));
			pos = ftell(f);
			err = fscanf(f, "%[^\n]", info);
			pos = ftell(f) - pos;
			info[pos] = '\0';
			info = (char*) realloc(info, (pos + 1) * sizeof(char));
			add_err = insert(&tbl, key, par, info);	
			free(info);
			if (!add_err) i++;
			
		} while (err != -1);
		free_table(tbl_curr);
		tbl_curr->csize = i;
		tbl_curr->msize = size;
		tbl_curr->arr = tbl.arr;
	}
	free(inp);
	fclose(f);
	return 1;
}

int dlg_save(Table* tbl)
{
	fseek(tbl->fd, 0, SEEK_SET);
	fwrite(&(tbl->msize), sizeof(int), 1, tbl->fd);
	fseek(tbl->fd, sizeof(int), SEEK_SET);
	fwrite(&(tbl->csize), sizeof(int), 1, tbl->fd);
	fseek(tbl->fd, sizeof(int) * 2, SEEK_SET);
	fwrite(tbl->arr, sizeof(Item), tbl->csize, tbl->fd);
	fclose(tbl->fd);
	tbl->fd = NULL;
	return 1;
}
