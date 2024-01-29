#include <stdio.h>

#include <int_array_1d.h>

/*
 * Minimum working example for int_array_1d.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 *   v1.1 2023-01-14: Added printouts at start/end of main.
 */

#define VERSION "v1.1"
#define VERSION_DATE "2023-01-14"

int main(void)
{
	printf("%s, %s %s: Create typed integer array.\n",
	       __FILE__, VERSION, VERSION_DATE);
	printf("Code base version %s.\n\n", CODE_BASE_VERSION);

	// Create an array with 6 positions.
	int_array_1d *a = int_array_1d_create(1,6);

	for (int i=int_array_1d_low(a); i<=int_array_1d_high(a); i++) {
		// Store square of index.
		int_array_1d_set_value(a,i*i,i);
		// Print array after setting each value.
		printf("After setting value at index %d:\n", i);
		int_array_1d_print(a);
	}

	int_array_1d_kill(a);

	printf("\nNormal exit.\n\n");
	return 0;
}
