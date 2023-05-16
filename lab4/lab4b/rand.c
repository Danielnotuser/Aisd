#include <stdlib.h>
#include <time.h>

#include "tree.h"
#include "rand.h"

static char *rand_str()
{
	int len = rand() % 40 + 1;
	char *s = (char*) calloc(len + 1, sizeof(char));
	for (int i = 0; i < len; i++)
		s[i] = rand() % 95 + 32;
	s[len] = '\0';
	return s;
}

void rand_tree(int num_node, int num_tree, int opened)
{
	srand(time(NULL));
	double avr_add_sum = 0, avr_del_sum = 0, avr_find_sum = 0;
	Tree tree;
	clock_t start, end;
	for (int j = 0; j < num_tree; j++)
	{
		tree.root = NULL;
		double sum_tm = 0;
		char **keys = (char**) calloc(num_node, sizeof(char*));
		for (int i = 0; i < num_node; i++)
		{
			char *key = rand_str();
			char *info = rand_str();
			keys[i] = key;
			start = clock();
			insert(&tree, key, info);
			end = clock();
			sum_tm += (double) (end - start) / CLOCKS_PER_SEC;
			free(info);
		}		
		avr_add_sum += sum_tm / num_node;
		int del_ind = rand() % num_node;
		Item del;
		start = clock();
		delete(&tree, tree.root, keys[del_ind], &del);
		end = clock();
		sum_tm += (double) (end - start) / CLOCKS_PER_SEC;
		avr_del_sum += sum_tm / num_node;
		Item fnd;
		int find_ind = rand() % num_node;
		start = clock();
		find(tree.root, keys[find_ind], &fnd);
		end = clock();
		sum_tm += (double) (end - start) / CLOCKS_PER_SEC;
		avr_find_sum += sum_tm / num_node;
		free_recur(&tree.root);
		for (int i = 0; i < num_node; i++)
			free(keys[i]);
		free(keys);
	}	
	FILE *fd;
	if (opened) fd = fopen("rand_results.txt", "r+");
	else fd = fopen("rand_results.txt", "w");
	fseek(fd, 0, SEEK_END);
	fprintf(fd, "num_node = %d, num_tree = %d\n", num_node, num_tree);
	printf("Average time for add: %.10lf\n", avr_add_sum / num_tree);
	fprintf(fd, "%.10lf\n", avr_add_sum / num_tree);
	printf("Average time for delete: %.10lf\n", avr_del_sum / num_tree);
	fprintf(fd, "%.10lf\n", avr_del_sum / num_tree);
	printf("Average time for find: %.10lf\n", avr_find_sum / num_tree);
	fprintf(fd, "%.10lf\n", avr_find_sum / num_tree);
	fclose(fd);
}
