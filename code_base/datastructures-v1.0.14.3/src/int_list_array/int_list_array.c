#include <stdio.h>
#include <stdlib.h>
#include <strings.h> // For bcopy

#include <int_list_array.h>

/*
 * Implementation of a typed, undirected list of integers for the
 * "Datastructures and algorithms" courses at the Department of
 * Computing Science, Umea University. The implementation uses a
 * static array.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   2018-01-28: v1.0. First public version.
 *   2023-01-19: v1.1. Added list_pos_equal and list_pos_is_valid functions. Bugfix
 *                     in list_remove.
 *   2023-01-20: v1.2. Renamed list_pos_equal to list_pos_are_equal.
 *   2023-03-23: v1.3. Renamed list_pos_are_equal to list_pos_is_equal.
 */

// ===========INTERNAL DATA TYPES============

/*
 * The list is implemented as a static array.
 */
struct list {
	int last_used_pos;
	int *values;
};

/*
 * Data structure interface
 */

#define ARRAY_MAX_SIZE 10000

/**
 * list_empty() - Create an empty list.
 *
 * Returns: A pointer to the new list.
 */
list *list_empty(void)
{
	// Allocate memory for the list head.
	list *l=malloc(sizeof(list));
	// Allocate memory for the elements.
	l->values=calloc(ARRAY_MAX_SIZE,sizeof(int));
	// Set last used position.
	l->last_used_pos=-1;
	return l;
}

/**
 * list_is_empty() - Check if a list is empty.
 * @l: List to check.
 *
 * Returns: True if the list is empty, otherwise false.
 */
bool list_is_empty(const list * l)
{
	// List is empty if no elements are used.
	return l->last_used_pos < 0;
}

/**
 * list_first() - Return the first position of a list, i.e. the
 *		  position of the first element in the list.
 * @l: List to inspect.
 *
 * Returns: The first position in the given list.
 */
list_pos list_first(const list * l)
{
	// First position is always 0.
	return 0;
}

/**
 * list_end() - Return the last position of a list, i.e. the position
 *		after the last element in the list.
 * @l: List to inspect.
 *
 * Returns: The last position in the given list.
 */
list_pos list_end(const list * l)
{
	// Last position is position *after* last used element.
	return l->last_used_pos + 1;
}

/**
 * list_next() - Return the next position in a list.
 * @l: List to inspect.
 * @p: Any valid position except the last in the list.
 *
 * Returns: The position in the list after the given position.
 *	    NOTE: The return value is undefined for the last position.
 */
list_pos list_next(const list * l, const list_pos p)
{
	if (list_pos_is_equal(l, p, list_end(l))) {
		// This should really throw an error.
		fprintf(stderr,"list_next: Warning: Trying to navigate "
			"past end of list!");
	}
	return p + 1;
}

/**
 * list_previous() - Return the previous position in a list.
 * @l: List to inspect.
 * @p: Any valid position except the first in the list.
 *
 * Returns: The position in the list before the given position.
 *	    NOTE: The return value is undefined for the first position.
 */
list_pos list_previous(const list * l, const list_pos p)
{
	if (list_pos_is_equal(l, p, list_first(l))) {
		// This should really throw an error.
		fprintf(stderr,"list_previous: Warning: Trying to navigate "
			"past beginning of list!\n");
	}
	return p - 1;
}

/**
 * list_inspect() - Return the value of the element at a given
 *		    position in a list.
 * @l: List to inspect.
 * @p: Any valid position in the list, except the last.
 *
 * Returns: The integer value stored in the element at postiion pos.
 *	    NOTE: The return value is undefined for the last position.
 */
int list_inspect(const list * l, const list_pos p)
{
	if (list_pos_is_equal(l, p, list_end(l))) {
		// This should really throw an error.
		fprintf(stderr,"list_inspect: Warning: Trying to inspect "
			"position at end of list!\n");
	}
	return l->values[p];
}

/**
 * list_insert() - Insert a new element with a given value into a list.
 * @l: List to manipulate.
 * @data: Integer value to be inserted into the list.
 * @pos: Position in the list before which the value should be inserted.
 *
 * Creates a new element and inserts it into the list before p.
 * Stores data in the new element.
 *
 * Returns: The position of the newly created element.
 */
list_pos list_insert(list * l, int data, const list_pos p)
{
	// Move elements at position pos and later forward.
	bcopy(l->values + p, l->values + p + 1,
		sizeof(int) * (l->last_used_pos - p + 1));

	// Set value.
	l->values[p] = data;

	// Increment number of used elements.
	l->last_used_pos++;

	// Return the position of the new value.
	return p;
}

/**
 * list_remove() - Remove an element from a list.
 * @l: List to manipulate.
 * @p: Position in the list of the element to remove.
 *
 * Removes the element at position p from the list. If a free_func
 * was registered at list creation, calls it to deallocate the memory
 * held by the element value.
 *
 * Returns: The position after the removed element.
 */
list_pos list_remove(list *l, const list_pos p)
{
	// Move elements at position pos and later forward.
	bcopy(l->values + p + 1, l->values + p, sizeof(int) * (l->last_used_pos - p));

	// Decrement number of used elements.
	l->last_used_pos--;

	// p now refers to the position after the removed element.
	return p;
}

/**
 * list_kill() - Destroy a given list.
 * @l: List to destroy.
 *
 * Returns all dynamic memory used by the list and its elements. If a
 * free_func was registered at list creation, also calls it for each
 * element to free any user-allocated memory occupied by the element values.
 *
 * Returns: Nothing.
 */
void list_kill(list * l)
{
	free(l->values);
	free(l);
}

/*
 * list_print() - Iterate over the list element and print their values.
 * @l: List to inspect.
 *
 * Iterates over the list and print each stored integer.
 *
 * Returns: Nothing.
 */
void list_print(const list * l)
{
	// Start at the beginning of the list.
	list_pos p = list_first(l);

	while (!list_pos_is_equal(l, p, list_end(l))) {
		// Call print_func with the element value at the
		// current position.
		printf("[%d]\n",list_inspect(l, p));
		p = list_next(l, p);
	}
}

/**
 * list_pos_is_equal() - Return true if two positions in a list are equal.
 * @l: List to inspect.
 * @p1: First position to compare.
 * @p2: Second position to compare.
 *
 * Returns: True if p1 and p2 refer to the same position in l, otherwise False.
 *	    NOTE: The result is defined only if p1 and p2 are valid positions in l.
 */
bool list_pos_is_equal(const list *l, const list_pos p1, const list_pos p2)
{
	// Since we don't need to check whether p1 or p2 are valid, we
	// only need to compare them directly.
	return p1 == p2;
}

/**
 * list_pos_is_valid() - Return true for a valid position in a list
 * @l: List to inspect.
 * @p: Any position.
 *
 * Returns: True if p is a valid position in the list, otherwise false.
 */
bool list_pos_is_valid(const list *l, const list_pos p)
{
	// The position is valid if it is between 0 and last_used_pos + 1, inclusive.
	return p >= 0 && p <= l->last_used_pos+1;

}
