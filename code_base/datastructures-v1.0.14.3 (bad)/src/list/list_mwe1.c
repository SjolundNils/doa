#include <stdlib.h>
#include <stdio.h>

#include <list.h>

/*
 * Minimum working example for list.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *	    Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 *   v1.1 2018-04-03: Split into versions with/without memhandler.
 *   v1.2 2023-01-14: Added printouts at start/end of main.
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
	printf("%s, %s %s: Create integer list without memfreehandler.\n",
	       __FILE__, VERSION, VERSION_DATE);
	printf("Code base version %s.\n\n", CODE_BASE_VERSION);

	// Create the list.
	list *l = list_empty(NULL);
	// Allocate space for one integer.
	int *val = malloc(sizeof(int));
	// Give it a value.
	*val = 5;

	// Insert the value first in the list.
	list_insert(l, val, list_first(l));

	printf("List after inserting one value:\n");
	list_print(l, print_ints);

	// Allocate space for another integer.
	val = malloc(sizeof(int));
	*val = 8;
	// Insert the value last in the list.
	list_insert(l, val, list_end(l));

	printf("List after inserting second value at the end:\n");
	list_print(l, print_ints);

	// Allocate space for a third integer.
	val = malloc(sizeof(int));
	*val = 2;
	// Insert the value at the second position in the list.
	list_insert(l, val, list_next(l, list_first(l)));

	printf("List after inserting a third value in the middle:\n");
	list_print(l, print_ints);

	// Remove first element.
	list_pos p=list_first(l);
	int *v=list_inspect(l, p);
	// Remove element from list.
	list_remove(l, p);
	// Free element content.
	free(v);

	printf("List after removing first element:\n");
	list_print(l, print_ints);

	// Empty the list.
	while (!list_is_empty(l)) {
		list_pos p=list_first(l);
		int *v=list_inspect(l,p);
		free(v);
		list_remove(l,p);
	}
	// Done, kill the list.
	list_kill(l);

	printf("\nNormal exit.\n\n");
	return 0;
}
