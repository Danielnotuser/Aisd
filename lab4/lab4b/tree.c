#include <stdlib.h>
#include <string.h>

#include "tree.h"

void free_item(Item *a)
{
	free(a->key);
	free(a->info);
	a->key = NULL;
	a->info = NULL;
}

static void free_node(Node **rt)
{
	for (int i = 0; i < (*rt)->num_i; i++)
		free_item(&(*rt)->items[i]);
	free(*rt);	
}

void free_recur(Node **rt)
{
	for (int i = 0; i < (*rt)->num_p; i++)
		if ((*rt)->ptr[i])
			free_recur(&((*rt)->ptr[i]));
	free_node(rt);
}

static Node *create_node(char *key, char *info)
{
	Node *new = (Node*) calloc(1, sizeof(Node));
	if (!new) return NULL;
	char *new_key = strdup(key);
	char *new_info = strdup(info);
	new->items[0].key = new_key;
	new->items[0].info = new_info;
	for (int i = 0; i < 4; i++) new->ptr[i] = NULL;
	new->par = NULL;
	new->num_i = 1;
	new->num_p = 0;
	return new;
}

static Node *find_ptr(Node *rt, char *key, int *ptr_ind)
{
	for (int i = 0; i < rt->num_i; i++)
	{
		int cmp = strcmp(key, rt->items[i].key);
		if (cmp < 0)
		{
			if (rt->ptr[i]) 
			{
				(*ptr_ind) = i;
				return rt->ptr[i];
			}
			else
			{
				(*ptr_ind) = i;
				return NULL;
			}
		}
		else if (cmp > 0 && (i == rt->num_p - 2 || !rt->items[i + 1].key))
		{
			if (rt->ptr[i + 1]) 
			{
				(*ptr_ind) = i + 1;
				return rt->ptr[i + 1];
			}
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

static int key_add(Node *rt, Node *new)
{
	rt->num_i++;
	if (new->num_p) rt->num_p += 1;
	for (int i = 0; i < rt->num_i - 1; i++)
	{
		int cmp = strcmp(new->items[0].key, rt->items[i].key);
		if (!cmp)	
		{
			rt->num_i--;
			return 1;
		}
		if (cmp < 0)
		{
			for (int j = rt->num_i - 1; j > i; j--)
			{
				rt->items[j].key = rt->items[j - 1].key;
				rt->items[j].info = rt->items[j - 1].info;
			}
			for (int j = rt->num_p - 1; j > i; j--)
				rt->ptr[j] = rt->ptr[j - 1];
			rt->items[i] = new->items[0];
			if (new->ptr[0])
			{
				rt->ptr[i] = new->ptr[0];
				new->ptr[0]->par = rt;
			}
			if (new->ptr[1])
			{
				rt->ptr[i + 1] = new->ptr[1];
				new->ptr[1]->par = rt;	
			}
			return 0;
		}
		else if (!rt->items[i + 1].key)
		{
			rt->items[i + 1] = new->items[0];
			if (new->ptr[0])
			{
				rt->ptr[i + 1] = new->ptr[0];
				new->ptr[0]->par = rt;
			}
			if (new->ptr[1])
			{
				rt->ptr[i + 2] = new->ptr[1];
				new->ptr[1]->par = rt;	
			}
			return 0;
		}
	}
	return 0;
}

static int split(Tree *tree, Node *rt, Node *new, int ind_prev, int ind)
{
	int i = 0;
	while (i < rt->num_i && strcmp(new->items[0].key, rt->items[i].key)) i++;
	if (i != rt->num_i) return 1;
	Node *first = create_node(rt->items[0].key, rt->items[0].info);
	Node *mid = create_node(rt->items[1].key, rt->items[1].info);
	Node *last = create_node(rt->items[2].key, rt->items[2].info);
	first->par = mid;
	last->par = mid;
	mid->par = rt->par;
	for (int i = 0; i < rt->num_p; i++)
	{
		if (i < 2)
		{
			if (i != ind_prev) 
			{
				first->ptr[first->num_p] = rt->ptr[i];
				rt->ptr[i]->par = first;
			}
			first->num_p++;
		}
		else
		{
			if (i != ind_prev) 
			{
				last->ptr[last->num_p] = rt->ptr[i];
				rt->ptr[i]->par = last;
			}
			last->num_p++;
		}
	}
	mid->ptr[0] = first;
	mid->ptr[1] = last;
	mid->num_p = 2;
	free_node(&rt);
	int cmp = strcmp(new->items[0].key, mid->items[0].key), err;
	if (cmp > 0)
		err = key_add(last, new);
	else
		err = key_add(first, new);
	if (err) return 1;
	if (!mid->par)
	{	
		tree->root = mid;
		return 0;
	}
	if (mid->par->num_i < 3)
	{
		key_add(mid->par, mid);
		free(mid);
		return 0;
	}
	else
	{
		mid->par->ptr[ind] = mid;
		int ind_new = 0;
		if (mid->par->par) find_ptr(mid->par->par, mid->par->items[0].key, &ind_new);
		split(tree, mid->par, mid, ind, ind_new);
		free(mid);
		return 0;
	}			
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
		if (rt->num_p > 0) 
		{
			int ind = 0;
			Node *ptr = find_ptr(rt, key, &ind);
			if (ind == -1)
				return 1;
			if (!ptr)
			{
				rt->ptr[ind] = create_node(key, info);
				rt->ptr[ind]->par = rt;
				return 0;
			}
			rt = ptr;
			continue;
		}
		else
		{
			int err;
			Node *new = create_node(key, info);
			if (rt->num_i == 3)
			{
				int ind = 0;
				if (rt->par) find_ptr(rt->par, key, &ind);
				err = split(tree, rt, new, ind, ind);
			}
			else
				err = key_add(rt, new);
			free(new);
			return err;
		}
	}
	
}

int find(Node *rt, char *key, Item *fnd)
{
	int err;
	for (int i = 0; i < rt->num_i; i++)
	{
		int cmp = strcmp(key, rt->items[i].key);
		if (cmp < 0)
		{
			if (rt->ptr[i]) 
				err = find(rt->ptr[i], key, fnd);
			else
			{
				fnd = NULL;
				return 1;
			}
			return err;
		}
		else if (cmp > 0 && (i == rt->num_p - 2 || !rt->items[i + 1].key))
		{
			if (rt->ptr[i + 1]) 
				err = find(rt->ptr[i + 1], key, fnd);
			else
			{
				fnd = NULL;
				return 1;
			}	
			return err;
		}
		else if (!cmp)
		{
			(*fnd) = rt->items[i];
			return 0;
		}
	}		
}

int special_find(Node *rt, char *key, Item *fnd)
{
	int err, j;
	for (int i = 0; i < rt->num_i; i++)
	{
		int cmp = strcmp(rt->items[i].key, key);
		if (cmp > 0)
		{
			if (i == 0)
			{
				if (!rt->num_p) 
				{
					if (!rt->par) return 1;
					else 
					{
						for (j = 0; j < rt->par->num_i; j++)
							if (strcmp(key, rt->par->items[i].key) > 0)
								break;
						(*fnd) = rt->par->items[j - 1];
						return 0;
					}
				}
				else
				{
					err = special_find(rt->ptr[0], key, fnd);
					return err;
				}
				
			}
			if (rt->num_p == 0 && i > 0 && strcmp(rt->items[i - 1].key, key) < 0)
			{
				(*fnd) = rt->items[i - 1];
				return 0;
			}
			if (rt->ptr[i]) 
				err = special_find(rt->ptr[i], key, fnd);
			else
				return 1;
			return err;
		}
		else if (cmp < 0)
		{
			if (i == rt->num_i - 1 || i == rt->num_p - 1)
			{
				if (rt->num_p == 0)
				{
					(*fnd) = rt->items[i];
					return 0;
				}
				if (rt->ptr[i + 1])
					err = special_find(rt->ptr[i + 1], key, fnd);
				else if (rt->ptr[i])
					err = special_find(rt->ptr[i], key, fnd);
				return err;
			}
		}
		else
		{
			(*fnd) = rt->items[i];
			return 0;
		}
	}		
}

int very_special_find(Node *rt, char *key, Item *fnd)
{
	int err, j;
	for (int i = 0; i < rt->num_i; i++)
	{
		int cmp = strcmp(rt->items[i].key, key);
		if (cmp < 0)
		{
			if (i == rt->num_i - 1)
			{
				if (rt->num_p - 1 <= i)
				{
					if (!rt->par) return 1;
					else 
					{
						for (j = 0; j < rt->par->num_i; j++)
							if (strcmp(key, rt->par->items[i].key) > 0)
								break;
						(*fnd) = rt->par->items[j];
						return 0;
					}
				}
				else
				{
					err = very_special_find(rt->ptr[rt->num_p - 1], key, fnd);
					return err;
				}
			}
		}
		else if (cmp > 0)
		{
			if (rt->num_p - 1 < i)
			{
				(*fnd) = rt->items[i];
				return 0;
			}
			else
			{
				err = very_special_find(rt->ptr[i], key, fnd);
				return err;
			}
		}
		else
		{
			(*fnd) = rt->items[i];
			return 0;
		}
	}		
}

static Item copy_item(Item a)
{
	Item b;
	b.key = strdup(a.key);
	b.info = strdup(a.info);
	return b;
}

static void shift_node(Node *rt, int ind)
{
	for (int i = ind; i < 4; i++)
	{
		if (i >= 0 && i < rt->num_i - 1)
			rt->items[i] = rt->items[i + 1];
		if (i >= 0 && i > ind && i < rt->num_p)
		{
			if (i == rt->num_p - 1) rt->ptr[i] = NULL;
			else rt->ptr[i] = rt->ptr[i + 1];
		}
	}	
	rt->num_i--;
	if (rt->num_p) rt->num_p--;
}

int delete(Tree *tree, Node *rt, char *key, Item *del)
{
	int k, y, res_ind, err;
	while (rt->num_p)
	{
		k = -1;
		for (int i = 0; i < rt->num_i; i++) 
		{
			int cmp = strcmp(key, rt->items[i].key);
			if (cmp < 0)
			{
				y = i;
				break;
			}
			else if (cmp > 0 && i == rt->num_i - 1)
				y = i + 1;
			else if (!cmp)
			{
				k = i;
				break;
			}
		}
		if (k < 0)
		{
			res_ind = y;
			if (rt->ptr[y]->num_i == 1)
			{   // два if для случая, когда слева и справа от y по 1 item
				if ((y == 0 || rt->ptr[y - 1]->num_i == 1) && (y < rt->num_p - 1 && rt->ptr[y + 1]->num_i == 1))
				{ // соединяем y с правым
					rt->ptr[y]->items[1] = copy_item(rt->items[y]);
					rt->ptr[y]->items[2] = copy_item(rt->ptr[y + 1]->items[0]);
					rt->ptr[y]->ptr[2] = rt->ptr[y + 1]->ptr[0];
					rt->ptr[y]->ptr[3] = rt->ptr[y + 1]->ptr[1];
					if (rt->ptr[y + 1]->ptr[0]) rt->ptr[y + 1]->ptr[0]->par = rt->ptr[y];
					if (rt->ptr[y + 1]->ptr[1]) rt->ptr[y + 1]->ptr[1]->par = rt->ptr[y];
					free_node(&rt->ptr[y + 1]);
					if (rt->num_i == 1)
					{
						if (rt->par)
						{
							int ind;
							find_ptr(rt->par, rt->items[0].key, &ind);
							rt->par->ptr[ind] = rt->ptr[y];
						}
						else
							tree->root = rt->ptr[y];
						free_node(&rt);
					}
					else
					{
						free_item(&rt->items[y]);
						shift_node(rt, y);
					}
					rt->ptr[y]->num_i += 2;
				}
				else if ((y > 0 && rt->ptr[y - 1]->num_i == 1) && (y == rt->num_p - 1 || rt->ptr[y + 1]->num_i == 1))
				{ // соединяем y с левым
					rt->ptr[y - 1]->items[1] = copy_item(rt->items[y - 1]);
					rt->ptr[y - 1]->items[2] = copy_item(rt->ptr[y]->items[0]);
					rt->ptr[y - 1]->ptr[2] = rt->ptr[y]->ptr[0];
					rt->ptr[y - 1]->ptr[3] = rt->ptr[y]->ptr[1];
					if (rt->ptr[y]->ptr[0]) rt->ptr[y]->ptr[0]->par = rt->ptr[y - 1];
					if (rt->ptr[y]->ptr[1]) rt->ptr[y]->ptr[1]->par = rt->ptr[y - 1];
					free_node(&rt->ptr[y]);
					if (rt->num_i == 1)
					{
						if (rt->par)
						{
							int ind;
							find_ptr(rt->par, rt->items[0].key, &ind);
							rt->par->ptr[ind] = rt->ptr[y - 1];
						}
						else
							tree->root = rt->ptr[y - 1];
						free_node(&rt);
					}
					else
					{
						free_item(&rt->items[y - 1]);
						shift_node(rt, y - 1);	
					}
					res_ind = y - 1;	
					rt->ptr[y - 1]->num_i += 2;			
				}
				// два if для случая, когда слева или справа от y >= 2 items
				else if (y > 0 && rt->ptr[y - 1]->num_i >= 2)
				{ // присоединяем к y слева
					rt->ptr[y]->items[1] = rt->ptr[y]->items[0];
					rt->ptr[y]->items[0] = copy_item(rt->items[y - 1]);
					free_item(&rt->items[y - 1]);
					rt->items[y - 1] = copy_item(rt->ptr[y - 1]->items[rt->ptr[y - 1]->num_i - 1]);
					rt->ptr[y]->ptr[2] = rt->ptr[y]->ptr[1];
					rt->ptr[y]->ptr[1] = rt->ptr[y]->ptr[0];
					rt->ptr[y]->ptr[0] = rt->ptr[y - 1]->ptr[rt->ptr[y - 1]->num_i];
					if (rt->ptr[y - 1]->ptr[rt->ptr[y - 1]->num_i]) rt->ptr[y - 1]->ptr[rt->ptr[y - 1]->num_i]->par = rt->ptr[y];
					rt->ptr[y - 1]->ptr[rt->ptr[y - 1]->num_i] = NULL;
					free_item(&rt->ptr[y - 1]->items[rt->ptr[y - 1]->num_i - 1]);
					rt->ptr[y]->num_i++;
					rt->ptr[y - 1]->num_i--;
					if (rt->ptr[y]->ptr[0])	
					{
						rt->ptr[y - 1]->num_p--;		
						rt->ptr[y]->num_p++;
					}	
				}
				else if (y < rt->num_p - 1 && rt->ptr[y + 1]->num_i >= 2)
				{ // присоединяем к y справа
					rt->ptr[y]->items[1] = copy_item(rt->items[y]);
					free_item(&rt->items[y]);
					rt->items[y] = copy_item(rt->ptr[y + 1]->items[0]);
					rt->ptr[y]->ptr[2] = rt->ptr[y + 1]->ptr[0];
					if (rt->ptr[y + 1]->ptr[0]) rt->ptr[y + 1]->ptr[0]->par = rt->ptr[y];
					free_item(&rt->ptr[y + 1]->items[0]);
					shift_node(rt->ptr[y + 1], -1);			
					rt->ptr[y]->num_i++;	
					if (rt->ptr[y]->ptr[2])	
					{
						rt->ptr[y]->num_p++;	
					}		
				}				
				
			}
			rt = rt->ptr[res_ind];
		}
		else
		{
			(*del) = copy_item(rt->items[k]);
			if (rt->ptr[k] && rt->ptr[k]->num_i >= 2)
			{
				Item fnd;
				special_find(rt->ptr[k], key, &fnd);
				free_item(&rt->items[k]);
				rt->items[k] = copy_item(fnd);
				Item del_2;
				char *fnd_key = strdup(fnd.key);
				err = delete(tree, rt->ptr[k], fnd_key, &del_2);
				free(fnd_key);
				free_item(&del_2);
				return err;
			}
			else if (k + 1 < rt->num_p  && rt->ptr[k + 1]->num_i >= 2)
			{
				Item fnd;
				rt->ptr[k + 1]->par = NULL;
				very_special_find(rt->ptr[k + 1], rt->items[k].key, &fnd);
				rt->ptr[k + 1]->par = rt;
				free_item(&rt->items[k]);
				rt->items[k] = copy_item(fnd);
				Item del_2;
				char *fnd_key = strdup(fnd.key);
				err = delete(tree, rt->ptr[k + 1], fnd_key, &del_2);
				free(fnd_key);
				free_item(&del_2);
				return err;
			}
			else
			{
				rt->ptr[k]->items[1] = copy_item(rt->items[k]);
				if (rt->ptr[k + 1])
				{
					rt->ptr[k]->items[2] = copy_item(rt->ptr[k + 1]->items[0]);
					rt->ptr[k]->ptr[2] = rt->ptr[k + 1]->ptr[0];
					rt->ptr[k]->ptr[3] = rt->ptr[k + 1]->ptr[1];
					if (rt->ptr[k + 1]->ptr[0]) rt->ptr[k + 1]->ptr[0]->par = rt->ptr[k]->ptr[2];
					if (rt->ptr[k + 1]->ptr[1]) rt->ptr[k + 1]->ptr[1]->par = rt->ptr[k]->ptr[3];
					free_node(&rt->ptr[k + 1]);
				}
				free_item(&rt->items[k]);
				shift_node(rt, k);
				rt->ptr[k]->num_i += 2;
				Item del_2;
				err = delete(tree, rt->ptr[k], key, &del_2);
				free_item(&del_2);
				return err;
			}
		}
	}
	for (int i = 0; i < rt->num_i; i++) 
	{
		int cmp = strcmp(key, rt->items[i].key);
		if (!cmp)
		{
			(*del) = copy_item(rt->items[i]);
			free_item(&rt->items[i]);
			shift_node(rt, i);
			return 0;
		}
	}
	return 1;
}

void detour(Node *rt)
{
	for (int i = 0; i < rt->num_i; i++)
		printf("%s ", rt->items[i].key);
	for (int i = 0; i < rt->num_p; i++)
		detour(rt->ptr[i]);
}

void print_node(FILE *fd, Node *rt)
{
	fprintf(fd, "subgraph cluster_%s {\n", rt->items[0].key);
	for (int i = 0; i < rt->num_i; i++)
		fprintf(fd, "\"%s\"; ", rt->items[i].key);
	fprintf(fd, "\n}\n");
	for (int i = 0; i < rt->num_p; i++)
	{
		if (i == rt->num_i)
			fprintf(fd, "\"%s\"->\"%s\";\n", rt->items[i - 1].key, rt->ptr[i]->items[0].key);
		else
			fprintf(fd, "\"%s\"->\"%s\";\n", rt->items[i].key, rt->ptr[i]->items[0].key);
		print_node(fd, rt->ptr[i]);
	}
	
}

