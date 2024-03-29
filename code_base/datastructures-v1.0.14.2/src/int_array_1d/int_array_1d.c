#include <stdio.h>
#include <stdlib.h>

#include <int_array_1d.h>

/*
 * Implementation of a generic 1D array for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *	    Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *	    Lars Karlsson (larsk@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 */

// ===========INTERNAL DATA TYPES============

struct int_array_1d {
	int low; // Pointer to low indices for each dimension.
	int high; // Pointer to high indices for each dimension.
	int array_size; // Number of array elements.
	int *values; // Pointer to where the actual values are stored.
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * int_array_1d_create() - Create an array without values.
 * @lo: low index limit.
 * @hi: high index limit.
 *
 * The index limits are inclusive, i.e. all indices i such that low <=
 * i <= high are defined.
 *
 * Returns: A pointer to the new array, or NULL if not enough memory
 * was available.
 */
int_array_1d *int_array_1d_create(int lo, int hi)
{
	// Allocate array structure.
	int_array_1d *a=calloc(1, sizeof(*a));
	// Store index limit.
	a->low=lo;
	a->high=hi;

	// Number of elements.
	a->array_size = hi-lo+1;

	a->values=calloc(a->array_size, sizeof(*(a->values)));

	// Check whether the allocation succeeded.
	if (a->values == NULL) {
		free(a);
		a=NULL;
	}
	return a;
}

/**
 * int_array_1d_low() - Return the low index limit for the array.
 * @a: array to inspect.
 *
 * Returns: The low index limit.
 */
int int_array_1d_low(const int_array_1d *a)
{
	return a->low;
}

/**
 * int_array_1d_high() - Return the high index limit for the array.
 * @a: array to inspect.
 *
 * Returns: The high index limit.
 */
int int_array_1d_high(const int_array_1d *a)
{
	return a->high;
}

/**
 * int_array_1d_inspect_value() - Inspect a value at a given array position.
 * @a: array to inspect.
 * @i: index of position to inspect.
 *
 * Returns: The element value at the specified position. The result is
 *	    undefined if no value are stored at that position.
 */
int int_array_1d_inspect_value(const int_array_1d *a, int i)
{
	int offset=i-int_array_1d_low(a);
	// Return the value.
	return a->values[offset];
}

/**
 * int_array_1d_has_value() - Check if a value is set at a given array position.
 * @a: array to inspect.
 * @i: index of position to inspect.
 *
 * Returns: True if a value is set at the specified position, otherwise false.
 */
bool int_array_1d_has_value(const int_array_1d *a, int i)
{
	int offset=i-int_array_1d_low(a);
	// Return true if the value is not NULL.
	return a->values[offset] != 0;
}

/**
 * int_array_1d_set_value() - Set a value at a given array position.
 * @a: array to modify.
 * @v: value to set element to, or 0 to clear value.
 * @i: index of position to modify.
 *
 * Returns: Nothing.
 */
void int_array_1d_set_value(int_array_1d *a, int v, int i)
{
	int offset=i-int_array_1d_low(a);
	// Set value.
	a->values[offset]=v;
}

/**
 * int_array_1d_kill() - Return memory allocated by array.
 * @a: array to kill.
 *
 * Iterates over all elements. If free_func was specified at array
 * creation, calls it for every non-NULL element value.
 *
 * Returns: Nothing.
 */
void int_array_1d_kill(int_array_1d *a)
{
	// Free actual storage.
	free(a->values);
	// Free array structure.
	free(a);
}

/**
 * int_array_1d_print() - Iterate over the array element and print their values.
 * @a: Array to inspect.
 * @print_func: Function called for each non-NULL element.
 *
 * Iterates over each position in the array. Calls print_func for each
 * non-NULL value.
 *
 * Returns: Nothing.
 */
void int_array_1d_print(const int_array_1d *a)
{
	printf("[ ");
	for (int i=int_array_1d_low(a); i<=int_array_1d_high(a); i++) {
		if (int_array_1d_has_value(a,i)) {
			printf("[%d]",int_array_1d_inspect_value(a,i));
		} else {
			printf(" []");
		}
		if (i<int_array_1d_high(a)) {
			printf(", ");
		}
	}
	printf(" ]\n");
}
