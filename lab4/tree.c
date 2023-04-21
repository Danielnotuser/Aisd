#include <stdlib.h>
#include <string.h>

#include "tree.h"

static void free_node(Node *rt)
{
	rt->par = NULL;
	rt->next = NULL;
	for (int i = 0; i < rt->info.n; i++)
		free(rt->info.arr[i]);
	free(rt->info.arr);
	free(rt->key);
	free(rt);	
}

void free_recur(Node *rt)
{
	Node *left = rt->left;
	Node *right = rt->right;
	free_node(rt);
	if (left) free_recur(left);
	if (right) free_recur(right);	
}

int insert(Tree *tree, char *key, char *info)
{
	if (!tree->root)
	{
		Item info_list;
		info_list.arr = (char**) calloc(1, sizeof(char*));
		if (!info_list.arr) return 1;
		info_list.n = 1;
		(info_list.arr)[0] = strdup(info);
		char *new_key = strdup(key);
		Node *new = (Node*) calloc(1, sizeof(Node));
		new->key = new_key; // new = &({new_key, info_list, NULL, NULL}) ?
		new->info = info_list;
		new->left = NULL;
		new->right = NULL;
		new->par = NULL;
		new->next = NULL;
		tree->root = new;
		return 0;
	}
	Node *rt = tree->root;
	while (1)
	{
		int cmp = strcmp(key, rt->key);
		if (!cmp)
		{
			rt->info.n++;
			rt->info.arr = (char**) realloc(rt->info.arr, rt->info.n * sizeof(char*));
			if (!rt->info.arr) return 1;
			rt->info.arr[rt->info.n - 1] = strdup(info);
			return 0;
		}
		else if ((cmp > 0 && rt->right == NULL) || (cmp < 0 && rt->left == NULL))
		{
			Item info_list;
			info_list.arr = (char**) calloc(1, sizeof(char*));
			if (!info_list.arr) return 1;
			info_list.n = 1;
			info_list.arr[0] = strdup(info);
			char *new_key = strdup(key);
			Node *new = (Node*) calloc(1, sizeof(Node));
			new->key = new_key;
			new->info = info_list;
			new->left = NULL;
			new->right = NULL;	
			new->par = rt;
			new->next = NULL;		
			if (cmp > 0) rt->right = new;
			else rt->left = new;
			return 0;
		}
		else if (cmp > 0)
			rt = rt->right;
		else
			rt = rt->left;
	}
	
}
static void find_right_min(Node *rt, Node **fnd, Node **par)
{
	(*par) = NULL;
	while (rt->left)
	{
		(*par) = rt;
		rt = rt->left;
	}
	(*fnd) = rt;
}

static void shift_arr(Item *a, int num)
{
	char *del = a->arr[num];
	for (int i = num; i < a->n - 1; i++)
		a->arr[i] = a->arr[i + 1]; 	
	free(del);
}

int delete(Tree *tree, char *key, int num)
{
	Node *rt = tree->root;
	int cmp, rpar;
	while ((cmp = strcmp(key, rt->key)))
	{
		if (cmp > 0 && rt->right)
		{
			rpar = 1;
			rt = rt->right;
		}
		else if (cmp < 0 && rt->left)
		{
			rpar = 0;
			rt = rt->left;
		}	
		else
			return 1;
	}
	if (rt->info.n <= num)
		return 2;
	if (rt->left && rt->right)
	{
		Node *fnd, *par;
		find_right_min(rt->right, &fnd, &par);
		Node *new = (Node*) calloc(1, sizeof(Node));
		new->key = strdup(fnd->key);
		new->info.n = fnd->info.n;
		new->info.arr = (char**) calloc(new->info.n, sizeof(char*));
		for (int i = 0; i < new->info.n; i++) new->info.arr[i] = strdup(fnd->info.arr[i]);
		new->left = rt->left;
		new->right = rt->right;
		if (new->left) new->left->par = new;
		if (new->right) new->right->par = new;
		new->par = rt->par;
		Node *rt_par = rt->par;
		if (rt->info.n > 1)
			shift_arr(&(rt->info), num);
		else
		{
			free_node(rt);
			if (!rt_par)
				tree->root = new;
			else if (rpar)
				rt_par->right = new;
			else
				rt_par->left = new;
		}
		if (par)
			rpar = 0;	
		else
		{
			fnd->par = new;
			rpar = 1;	
		}
		rt = fnd;
	}
	if (!rt->left && !rt->right)
	{
		if (!rt->par) 
			tree->root = NULL;
		else if (rpar)
			rt->par->right = NULL;
		else
			rt->par->left = NULL;
	}
	else if ((rt->left && !rt->right) || (!rt->left && rt->right))
	{
		if (!rt->par)
		{
			if (rt->left) 
			{
				tree->root = rt->left;
				rt->left->par = tree->root;
			}
			else 
			{
				tree->root = rt->right;
				rt->right->par = tree->root;
			}
		}
		else if (rpar)
		{
			if (rt->left)
			{
				if (rpar)
				{
					rt->par->right = rt->left;
					rt->left->par = rt->par;
				}
				else
				{
					rt->par->left = rt->left;
					rt->left->par = rt->par;	
				}
			}
			else
			{
				if (rpar)
				{
					rt->par->right = rt->right;
					rt->right->par = rt->par;
				}
				else
				{
					rt->par->left = rt->right;
					rt->right->par = rt->par;	
				}	
			}
		}
	}
	if (rt->info.n > 1)
		shift_arr(&(rt->info), num);
	else
		free_node(rt);
	return 0;
}

int find(Tree *tree, char *key, int num, Node **fnd)
{
	Node *rt = tree->root;
	int cmp;
	while ((cmp = strcmp(key, rt->key)))
	{
		if (cmp > 0 && rt->right)
			rt = rt->right;
		else if (cmp < 0 && rt->left)
			rt = rt->left;
		else
			return 1;
	}
	if (rt->info.n <= num)
		return 2;
	(*fnd) = rt;
	return 0;
}

int special_find(Tree *tree, char *key, int num, Node **fnd)
{
	Node *rt_old = NULL;
	Node *rt = tree->root;
	int cmp;
	do
	{
		cmp = strcmp(rt->key, key);
		if (cmp > 0)
		{
			rt_old = rt;
			if (!rt->left)
				break;
			rt = rt->left;
		}
		else
		{
			if (!rt->right)
				break;
			rt = rt->right;
		}
			
	} while (1);
	if (!rt_old) return 1;
	if (rt_old->info.n <= num)
		return 2;
	(*fnd) = rt_old;
	return 0;
}

void thread(Tree *tree)
{
	Node *rt = tree->root;
	while (1)
	{
		if (rt->next) rt->next = NULL;
		if (rt->left || rt->right)
		{
			if (rt->left) rt = rt->left;
			else rt = rt->right;
		}
		else
		{
			Node *par = rt->par;
			Node *par_prev = rt->par;
			if (par->right == rt) par = par->par;
			while (par)
			{
				if ((par->right) && (par->right != par_prev))
				{
					rt->next = par->right;
					break;
				}
				else
				{	
					par_prev = par;
					par = par->par;
				}
			}
			if (!par) 
				break;
			rt = rt->next;
		}
		
	}
}

void detour(Tree *tree)
{
	Node *rt = tree->root;
	printf("%s ", rt->key);
	while (rt)
	{
		if (rt->left || rt->right)
		{
			if (rt->left) rt = rt->left;
			else rt = rt->right;
			printf("%s ", rt->key);
		}
		else
		{
			if (!rt->next) break;
			rt = rt->next;
			printf("%s ", rt->key);
		}
	}
}

/*void show_recur(Node *rt, int tab_num, char branch)
{
	if (tab_num >= 1) printf("|-");
	for (int i = 1; i < tab_num; i++) printf("--");
	if (branch == 'L' || branch == 'R') printf("%c", branch);
	printf("%s\n", rt->key);
	if (rt->left)
		show_recur(rt->left, tab_num + 1, 'L');
	if (rt->right)
		show_recur(rt->right, tab_num + 1, 'R');
}*/


void print_node(FILE *fd, Node *rt)
{
	Node *left = rt->left, *right = rt->right;
	if (left && right) fprintf(fd, "{ rank = same; \"%s\"; \"%s\"; }\n", left->key, right->key);
	if (left) fprintf(fd, "\"%s\"->\"%s\";\n", rt->key, left->key);
	if (right) fprintf(fd, "\"%s\"->\"%s\";\n", rt->key, right->key);
	if (left) print_node(fd, left);
	if (right) print_node(fd, right);

}

