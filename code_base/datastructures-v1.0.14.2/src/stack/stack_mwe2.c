#include <stdio.h>
#include <stdlib.h>

#include <stack.h>

/*
 * Minimum working example for stack.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *	    Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 *   v1.1 2018-04-03: Split into versions with/without memhandler.
 *   v1.2 2022-03-24: Update to always take care of returned pointer.
 *   v1.3 2023-01-14: Added printouts at start/end of main.
 */

#define VERSION "v1.3"
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
	printf("%s, %s %s: Create integer stack with standard memfreehandler.\n",
	       __FILE__, VERSION, VERSION_DATE);
	printf("Code base version %s.\n\n", CODE_BASE_VERSION);

	// Create the stack. Make the stack responsible for
	// deallocation pushed values.
	stack *s = stack_empty(free);
	for (int i=1; i<=3; i++) {
		// Allocate memory for one int.
		int *v = malloc(sizeof(*v));
		// Set value.
		*v=i;
		// Push value on stack.
		s = stack_push(s, v);
	}

	printf("--STACK before popping--\n");
	stack_print(s, print_ints);

	s = stack_pop(s);

	printf("--STACK after popping--\n");
	stack_print(s, print_ints);

	stack_kill(s);

	printf("\nNormal exit.\n\n");
	return 0;
}
