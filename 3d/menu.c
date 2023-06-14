#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

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
	if (strlen(fname) < 5 || strcmp(strrchr(fname, '.'), ".bin"))
	{
		printf("Error! Wrong file name.\n");
		free(fname);
		return 0;
	}
	err = load(tbl, fname);
	if (err == ENOENT)
	{
		printf("Enter size of table: ");
		err = read_nat(&sz);
		if (err)
		{
			free(fname);
			return 0;
		}
		err = create(tbl, fname, sz);
		if (err) 
		{
			perror("Error! ");
			free(fname);
			return 0;
		}
	}
	else if (err != 0)
	{
		perror("Error! ");
		free(fname);
		return 0;
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
		if (var >= n) printf("Error! Number must be in range of options. Try again...\n");
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
	const char *errmsgs[] = {"Item added.", "Table overflow.", 
							"Memory allocation error."};
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
	char *info;
	printf("Enter info: ");
	err = read_str(&info);
	if (err)
	{
		printf("\nInput has been interrupted.\n");
		return 0;
	}
	info = (char*) realloc(info, strlen(info) + 1);
	err = insert(tbl, key, info);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	free(info);
	return 1;
}

int dlg_find(Table *tbl)
{
	const char *errmsgs[] = {"Ok.", "There is no item with this key.", 
							"Memory allocation error."};
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
	Table res;
	err = find(tbl, key, &res);
	if (!err)
	{
		printf("Found releases of that key:\n");
		print(&res);
		free_table(&res);
	}
	else
		printf("Error! %s\n", errmsgs[err]);
	return 1;
}

int dlg_delete(Table *tbl)
{
	const char *errmsgs[] = {"Item removed.", "There is no item with this key."};
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
	char *fname;
	printf("Enter file name: ");
	int err = read_str(&fname);
	if (err)
	{
		printf("Error! Memory allocation error.\n");
		return 1;
	}
	fname = (char*) realloc(fname, strlen(fname) + 1);
	if (strcmp(strrchr(fname, '.'), ".txt"))
	{
		printf("Error! Wrong file name.\n");
		return 2;
	}
	FILE *f = fopen(fname, "r");
	if (f == NULL)
	{
		printf("Error! Can't open file \"%s\"", fname);
		return 3;
	}
	int size;
	unsigned int key;
	err = fscanf(f, "%d%*c", &size);
	if (err == 1)
	{
		char *info;
		int pos, add_err, i = 0;
		Table tbl;
		tbl.fd = tbl_curr->fd;
		int fd = fileno(tbl.fd);
		fflush(tbl.fd);
		ftruncate(fd, 0);
		err = create(&tbl, NULL, size);
		if (err)
			return 1;
		do
		{
			err = fscanf(f, "%u%*c", &key);
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
			add_err = insert(&tbl, key, info);	
			free(info);
			if (!add_err) i++;
			
		} while (err != -1);
		free_table(tbl_curr);
		tbl_curr->csize = i;
		tbl_curr->msize = size;
		tbl_curr->arr = tbl.arr;
	}
	free(fname);
	fclose(f);
	return 1;
}

int dlg_save(Table* tbl)
{
	fseek(tbl->fd, 0, SEEK_SET);
	fwrite(&(tbl->msize), sizeof(int), 1, tbl->fd);
	fwrite(&(tbl->csize), sizeof(int), 1, tbl->fd);
	fwrite(tbl->arr, sizeof(Item), tbl->msize, tbl->fd);
	fclose(tbl->fd);
	tbl->fd = NULL;
	return 1;
}
