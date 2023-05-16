#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "menu.h"
#include "tree.h"
#include "rand.h"

int menu(const char *opts[], int num_opts)
{
	int var = -1;
	int i, err;
	do
	{
		if (var >= num_opts) printf("Error! Number must be in range of options. Try again...\n");
		for (i = 0; i < num_opts; i++)
			printf("%s\n", opts[i]);
		printf("Choose the option: ");
		err = read_nat(&var);
		if (err)
			var = 0;
	} while (var >= num_opts);
	
	return var;
}

int dlg_add(Tree* tree)
{
	char *errmsgs[] = {"Node added.", "Duplicate key.", "Memory allocation error."};
	char *key;
	char *info;
	printf("Enter key: ");
	int err = read_str(&key);
	if (err) 
	{
		printf("\nInput has been interrupted.\n");
		return 1;
	}
	printf("Enter info: ");
	err = read_str(&info);
	if (err) 
	{
		free(key);
		printf("\nInput has been interrupted.\n");
		return 1;
	}
	key = (char*) realloc(key, strlen(key) + 1);
	info = (char*) realloc(info, strlen(info) + 1);
	err = insert(tree, key, info);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	free(info);
	free(key);
	return 0;	
}

int dlg_delete(Tree *tree)
{
	char *errmsgs[] = {"Node deleted.", "There is no such key."};
	char *key;
	printf("Enter key: ");
	int err = read_str(&key);
	if (err) 
	{
		printf("\nInput has been interrupted.\n");
		return 1;
	}
	key = (char*) realloc(key, strlen(key) + 1);
	Item del;
	err = delete(tree, tree->root, key, &del);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	if (!err)
		printf("key = %s, info = %s\n", del.key, del.info);
	free_item(&del);
	free(key);	
	return 0;
}

int dlg_find(Tree *tree)
{
	char *errmsgs[] = {"Node found.", "There is no such key."};
	char *key;
	printf("Enter key: ");
	int err = read_str(&key);
	if (err) 
	{
		printf("\nInput has been interrupted.\n");
		return 1;
	}
	key = (char*) realloc(key, strlen(key) + 1);
	Item fnd;
	err = find(tree->root, key, &fnd);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	if (!err)
		printf("key = %s, info = %s\n", key, fnd.info);
	free(key);
	return 0;	
}

int dlg_special_find(Tree *tree)
{
	char *errmsgs[] = {"Node found.", "There is no such key."};
	char *key;
	printf("Enter key: ");
	int err = read_str(&key);
	if (err) 
	{
		printf("\nInput has been interrupted.\n");
		return 1;
	}
	key = (char*) realloc(key, strlen(key) + 1);
	Item fnd;
	err = special_find(tree->root, key, &fnd);
	if (err) 
		printf("Error! ");
	printf("%s\n", errmsgs[err]);
	if (!err)
		printf("key = %s, info = %s\n", fnd.key, fnd.info);
	free(key);
	return 0;	
}


int dlg_import(Tree *tree_old)
{
	char *fname;
	printf("Enter file name: ");
	int err = read_str(&fname);
	if (err)
	{
		printf("\nInput has been interrupted.\n");
		return 1;
	}	
	fname = (char*) realloc(fname, strlen(fname) + 1);
	char *dot = strrchr(fname, '.');
	if (!dot || strcmp(dot, ".txt"))
	{
		free(fname);
		printf("Error! Wrong file name.\n");
		return 0;
	}
	FILE *fd = fopen(fname, "r");
	Tree tree;
	tree.root = NULL;
	char *key, *info;
	int pos;
	do
	{
		key = (char*) calloc(30, sizeof(char));
		info = (char*) calloc(30, sizeof(char));
		pos = ftell(fd);
		err = fscanf(fd, "%s", key);
		pos = ftell(fd) - pos;
		if (err == EOF) 
		{
			free(key);
			free(info);
			break;
		}
		key[pos] = '\0';
		key = (char*) realloc(key, pos + 1);
		pos = ftell(fd);
		err = fscanf(fd, "%s", info);
		pos = ftell(fd) - pos;
		if (err == EOF) 
		{
			free(key);
			free(info);
			break;
		}
		info[pos] = '\0';
		info = (char*) realloc(info, pos + 1);
		insert(&tree, key, info);
		free(key);
		free(info);
	} while (err != EOF);
	fclose(fd);
	free(fname);
	free_tree(tree_old);
	(*tree_old) = tree;
	return 0;
}

int dlg_detour(Tree *tree)
{
	if (!tree->root)
	{
		printf("Tree is empty.\n");
		return 0;
	}
	detour(tree->root);
	printf("\n");
	return 0; 	
}

int dlg_show(Tree *tree)
{
	if (!tree->root)
	{
		printf("Tree is empty.\n");
		return 0;
	}
	FILE *fd = fopen("tree.dot", "w");
	fprintf(fd, "digraph Tree {\n");
	print_node(fd, tree->root);
	fprintf(fd, "}");
	fclose(fd);
	system("dot -Tpng tree.dot -o tree.png");
	system("catimg tree.png");
	return 0;
}


int dlg_rand(int *opened)
{
	int num_node, num_tree;
	printf("Enter number of nodes: ");
	int err = read_nat(&num_node);
	if (err)
	{
		printf("\nInput has been intrerrupted\n");
		return 1;
	}
	printf("Enter number of trees: ");
	err = read_nat(&num_tree);
	if (err)
	{
		printf("\nInput has been intrerrupted\n");
		return 1;
	}
	rand_tree(num_node, num_tree, *opened);
	(*opened) = 1;
	return 0;
}

void free_tree(Tree *tree)
{
	if (tree->root)
		free_recur(&tree->root);
}
