#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <array_2d.h>

/*
 * Minimum working example for array_2d.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2018-04-03: Split into versions with/without memhandler.
 *   v1.2  2023-01-14: Added printouts at start/end of main.
 */

#define VERSION "v1.2"
#define VERSION_DATE "2023-01-14"

// Integers are stored via int pointers stored as void pointers.
// Convert the given pointer and print the dereferenced value.
static void print_ints(const void *data)
{
	const int *v = data;
	printf("[%d]", *v);
}

int main(void)
{
	printf("%s, %s %s: Create 4-by-3 array of integers without memfreehandler.\n",
	       __FILE__, VERSION, VERSION_DATE);
	printf("Code base version %s.\n\n", CODE_BASE_VERSION);

	// Create a 4-by-3 array.
	array_2d *a = array_2d_create(1,4,1,3,NULL);

	for (int i=array_2d_low(a,1); i<=array_2d_high(a,1); i++) {
		for (int j=array_2d_low(a,2); j<=array_2d_high(a,2); j++) {
			// Allocate memory for an integer.
			int *v=malloc(sizeof(*v));
			// Set value to square of index.
			*v=i*10+j;
			array_2d_set_value(a,v,i,j);
		}
	}
	printf("After filling the array with values:\n");
	array_2d_print(a, print_ints);

	// Empty the array.
	for (int i=array_2d_low(a,1); i<=array_2d_high(a,1); i++) {
		for (int j=array_2d_low(a,2); j<=array_2d_high(a,2); j++) {
			if (array_2d_has_value(a,i,j)) {
				int *v=array_2d_inspect_value(a,i,j);
				free(v);
			}
		}
	}
	// Return remaining memory.
	array_2d_kill(a);

	printf("\nNormal exit.\n\n");
	return 0;
}
