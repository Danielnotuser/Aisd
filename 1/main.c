#include <stdio.h>
#include <stdlib.h>
#include "line.h"

int main()
{
	Line *matrix;
	int m;
	printf("Write number of lines: ");
	int err = read_len(&m);
	if (err)
		return 1;
	matrix = (Line*) calloc(m, sizeof(Line));
	if (!matrix)
	{
		fprintf(stderr, "Error! Memory allocation error.");
		return 1;
	}
	int avr;
	err = input_matrix(&matrix, m, &avr);
	if (err)
		return 1;
	Line *res = fix_matrix(&matrix, m, avr);
	printf("Entered matrix: \n");
	print_matrix(matrix, m);
	free_matrix(&matrix, m);
	printf("Fixed matrix: \n");
	print_matrix(res, m);
	free_matrix(&res, m);
	return 0;
}
