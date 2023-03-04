#include <stdio.h>
#include <stdlib.h>
#include "line.h"

int read_val(int *n)
{
	int err;
	char end;
	do
	{
		err = scanf("%d%c", n, &end);
		if (err == -1) 
		{
			printf("\nInput has been interrupted.\n");
			return 1;
		}
		if ((err == 2) && (end == '\n'))
			return 0;
		printf("Error! Type of entered value is wrong.\n");
		printf("Write number again: ");
		scanf("%*[^\n]");
	} while (1);
}

int read_len(int *n)
{
	int err;
	do
	{
		err = read_val(n);
		if (err)
			return 1;
		if (*n <= 0)
			printf("Error! Number of elements must be greater than zero.\n");
		else
			return 0;
		printf("Write number again: ");
	}	while(1);	
}

int num_len(int a)
{
	int i = 0;
	while (a)
	{
		a /= 10;
		i++;
	}
	return i;
}

int input_matrix(Line **matr, int m, int *avr)
{
	int n, err, len_sum = 0, num_elem = 0;
	for (int i = 0; i < m; i++)                             
	{                                                       
	    printf("Write number of elements: ");               
	    err = read_len(&n);                                 
	    if (err)                                            
	    {                                                   
	        free_matrix(matr, i);                        
	        return 1;                                       
	    }                                                   
	    (*matr)[i].len = n;                                  
	    (*matr)[i].arr = (int*) calloc(n, sizeof(int));      
	    num_elem += n;                                      
	    for (int j = 0; j < n; j++)                         
	    {                                                   
	        printf("%d element: ", j);                      
	        read_val(&(*matr)[i].arr[j]);                    
	        len_sum += num_len((*matr)[i].arr[j]);           
	    }                                                   
	}    
	(*avr) = len_sum / num_elem;
	return 0;
}
                                                                                                       
Line *fix_matrix(Line **matr, int m, int avr)
{                          
	int k, n;                                                  
	Line *res = (Line*) calloc(m, sizeof(Line));            
	for (int i = 0; i < m; i++)                             
	{                                                       
	    k = 0;                                              
	    n = (*matr)[i].len;                                  
	    int *new_arr = (int*) calloc(n, sizeof(int));       
	    for (int j = 0; j < n; j++)                         
	    {                                                   
	        if (num_len((*matr)[i].arr[j]) > avr)            
	        {                                               
	            new_arr[k] = (*matr)[i].arr[j];              
	            k++;                                        
	        }                                               
	    }                                                   
	    new_arr = (int*) realloc(new_arr, k * sizeof(int)); 
	    res[i].len = k;                                     
	    res[i].arr = new_arr;                               
	}   
	return res;
}

void print_matrix(Line *matr, int len)
{
	int n;
	for (int i = 0; i < len; i++)
	{
		n = matr[i].len;
		if (n == 0) 
		{
			printf("\n");
			continue;
		}
		for (int j = 0; j < n; j++)
		{
			if (j == n - 1) printf("%d\n", matr[i].arr[j]);
			else printf("%d ", matr[i].arr[j]);
		}
	}
}

void free_matrix(Line **matr, int len)
{
	for (int i = 0; i < len; i++)
	{
		if ((*matr)[i].len != 0)
		{
			free((*matr)[i].arr);
		}
	}
	free(*matr);
}
