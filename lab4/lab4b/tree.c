#include <stdlib.h>
#include <string.h>

#include "tree.h"

static void free_node(Node **rt)
{
	for (int i = 0; i < (*rt)->n; i++)
	{
		if ((*rt)->keys[i])
		{
			free((*rt)->info[i]);
			free((*rt)->keys[i]);
		}
	}
	free(*rt);	
}

void free_recur(Node **rt)
{
	for (int i = 0; i < (*rt)->n; i++)
	{
		if ((*rt)->ptr[i])
			free_recur(&((*rt)->ptr[i]));
	}
	free_node(rt);
}

static Node *create_node(char *key, char *info)
{
	Node *new = (Node*) calloc(1, sizeof(Node));
	if (!new) return NULL;
	char *new_key = strdup(key);
	char *new_info = strdup(info);
	new->keys[0] = new_key;
	new->info[0] = new_info;
	for (int i = 0; i < 4; i++) new->ptr[i] = NULL;
	new->n = 1;
	return new;
}

static Node *find_ptr(Node *rt, char *key, int *ptr_ind)
{
	for (int i = 0; i < rt->n; i++)
	{
		if (rt->keys[i])
		{
			int cmp = strcmp(key, rt->keys[i]);
			if (cmp < 0)
			{
				if (rt->ptr[i]) 
					return rt->ptr[i];
				else
				{
					(*ptr_ind) = i;
					return NULL;
				}
			}
			else if (cmp > 0 && (i == rt->n - 1 || !rt->keys[i + 1]))
			{
				if (rt->ptr[i + 1]) 
					return rt->ptr[i + 1];
				else
				{
					(*ptr_ind) = i + 1;
					return NULL;
				}	
			}
			else if (!cmp)
			{
				(*ptr_ind) = -1;
				return NULL;
			}
		}
	}		
}

static int key_add(Node *rt, char *key, char *info)
{
	rt->n++;
	for (int i = 0; i < rt->n - 1; i++)
	{
		int cmp = strcmp(key, rt->keys[i]);
		if (!cmp)	
			return 1;
		if (cmp < 0)
		{
			for (int j = i; j < rt->n; j++)
			{
				rt->keys[j + 1] = rt->keys[j];
				rt->info[j + 1] = rt->info[j];
				rt->ptr[j + 1] = rt->ptr[j];
			}
			rt->keys[i] = strdup(key);
			rt->info[i] = strdup(info);
			return 0;
		}
		else if (!rt->keys[i + 1])
		{
			rt->keys[i + 1] = strdup(key);
			rt->info[i + 1] = strdup(info);
			return 0;
		}
		
	}
	return 0;
}

int insert(Tree *tree, char *key, char *info)
{
	if (!tree->root)
	{
		Node *new = create_node(key, info);
		if (!new) return 2;
		tree->root = new;
		return 0;
	}
	Node *rt = tree->root;
	while (1)
	{
		if (rt->ptr[0]) 
		{
			int ind = 0;
			Node *ptr = find_ptr(rt, key, &ind);
			if (ind == -1)
				return 1;
			if (!ptr)
			{
				rt->ptr[ind] = create_node(key, info);
				return 0;
			}
			rt = ptr;
			continue;
		}
		else
		{
			if (rt->n == 4)
				split();
			else
			{
				int err = key_add(rt, key, info);
				return err;
			}
		}
			
		
	}
	
}

void print_node(FILE *fd, Node *rt)
{
	while (1)
	{
		if (rt->left && rt->right) fprintf(fd, "{ rank = same; \"%s\"; \"%s\"; }\n", rt->left->key, rt->right->key);
		if (rt->left) fprintf(fd, "\"%s\"->\"%s\";\n", rt->key, rt->left->key);
		if (rt->right) fprintf(fd, "\"%s\"->\"%s\";\n", rt->key, rt->right->key);
		if (rt->left || rt->right)
		{
			if (rt->left) rt = rt->left;
			else rt = rt->right;
		}
		else
		{
			if (!rt->next) break;
			rt = rt->next;
		}
	}
}

