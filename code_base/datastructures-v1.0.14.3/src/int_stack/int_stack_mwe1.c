#include <stdio.h>
#include <stdlib.h>

#include <int_stack.h>

/*
 * Minimum working example for int_stack.c.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2022-03-24: First public version.
 *   v1.1 2023-01-14: Added printouts at start/end of main.
 */

#define VERSION "v1.1"
#define VERSION_DATE "2023-01-14"

int main(void)
{
	printf("%s, %s %s: Create typed integer stack.\n",
	       __FILE__, VERSION, VERSION_DATE);
	printf("Code base version %s.\n\n", CODE_BASE_VERSION);

	// Create the stack.
	stack s=stack_empty();

	for (int i=1; i<=3; i++) {
	    // Push value on stack.
	    s = stack_push(s, i);
	}

	printf("--STACK before popping--\n");
	stack_print(s);

	// Pop element from stack.
	s=stack_pop(s);

	printf("--STACK after popping--\n");
	stack_print(s);

	// Kill stack. Strictly not necessary for int_stack since it
	// does not use any dynamic memory. Included for symmetry with
	// stack.c.
	stack_kill(s);

	printf("\nNormal exit.\n\n");
	return 0;
}
