#ifndef __INT_ARRAY_1D_H
#define __INT_ARRAY_1D_H

#include <stdbool.h>
#include "util.h"

/*
 * Declaration of an integer 1D array for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. After use, the function array_kill must be called to
 * de-allocate the dynamic memory used by the array itself.
 *
 * An element value of 0 is considered to be "no" value.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   2018-01-28: v1.0, first public version.
 */

// ==========PUBLIC DATA TYPES============

// List type.
typedef struct int_array_1d int_array_1d;

// ==========DATA STRUCTURE INTERFACE==========

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
int_array_1d *int_array_1d_create(int lo, int hi);

/**
 * int_array_1d_low() - Return the low index limit for the array.
 * @a: array to inspect.
 *
 * Returns: The low index limit.
 */
int int_array_1d_low(const int_array_1d *a);

/**
 * int_array_1d_high() - Return the high index limit for the array.
 * @a: array to inspect.
 *
 * Returns: The high index limit.
 */
int int_array_1d_high(const int_array_1d *a);

/**
 * int_array_1d_inspect_value() - Inspect a value at a given array position.
 * @a: array to inspect.
 * @i: index of position to inspect.
 *
 * Returns: The element value at the specified position, or 0 if no
 *	    value is stored at that position.
 */
int int_array_1d_inspect_value(const int_array_1d *a, int i);

/**
 * int_array_1d_has_value() - Check if a value is set at a given array position.
 * @a: array to inspect.
 * @i: index of position to inspect.
 *
 * Returns: True if a value is set at the specified position, otherwise false.
 */
bool int_array_1d_has_value(const int_array_1d *a, int i);

/**
 * int_array_1d_set_value() - Set a value at a given array position.
 * @a: array to modify.
 * @v: value to set element to, or 0 to clear value.
 * @i: index of position to modify.
 *
 * Returns: Nothing.
 */
void int_array_1d_set_value(int_array_1d *a, int v, int i);

/**
 * int_array_1d_kill() - Return memory allocated by array.
 * @a: array to kill.
 *
 * Iterates over all elements. If free_func was specified at array
 * creation, calls it for every non-NULL element value.
 *
 * Returns: Nothing.
 */
void int_array_1d_kill(int_array_1d *a);

/**
 * int_array_1d_print() - Iterate over the array element and print their values.
 * @a: Array to inspect.
 *
 * Iterates over each position in the array. Prints each non-zero element.
 *
 * Returns: Nothing.
 */
void int_array_1d_print(const int_array_1d *a);

#endif
