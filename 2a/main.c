#include <stdlib.h>
#include <string.h>

#include "func.h"

int main()
{
	int n, len_man;
	printf("Write number of reseptions: ");
	int err = read_nat(&n, stdin, stdout);
	if (err)
		return 1;
	Man **arr_man = man_init(&len_man, stdin, stdout);
	if (!arr_man)
		return 1;
	int (*cmp) (const Man**, const Man**) = man_cmp;
	if (len_man > 1)
		qsort(arr_man, len_man, sizeof(Man*), (int (*) (const void *, const void *)) cmp);
	err = queue_init(n, arr_man, len_man, stdout);
	if (err)
		return 1;
	free(arr_man);
	return 0;
}
