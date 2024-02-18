#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <table.h>
#include <dlist.h>

/*
 * Implementation of a generic table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Duplicates are handled by inspect and remove.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *	    Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0 2018-02-06: First public version.
 *   v1.1 2019-02-21: Second version without dlist/memfreehandler.
 *   v1.2 2019-03-04: Bugfix in table_remove.
 */

// ===========INTERNAL DATA TYPES============

struct table {
	dlist *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
};

struct table_entry {
	void *key;
	void *value;
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * table_empty() - Create an empty table.
 * @key_cmp_func: A pointer to a function to be used to compare keys.
 * @key_free_func: A pointer to a function (or NULL) to be called to
 *		   de-allocate memory for keys on remove/kill.
 * @value_free_func: A pointer to a function (or NULL) to be called to
 *		     de-allocate memory for values on remove/kill.
 *
 * Returns: Pointer to a new table.
 */
table *table_empty(compare_function *key_cmp_func,
		   free_function key_free_func,
		   free_function value_free_func)
{
	// Allocate the table header.
	table *t = calloc(1, sizeof(table));
	// Create the list to hold the table_entry-ies.
	t->entries = dlist_empty(NULL);
	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;

	return t;
}

/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Returns: True if table contains no key/value pairs, false otherwise.
 */
bool table_is_empty(const table *t)
{
	return dlist_is_empty(t->entries);
}

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. No test is performed to
 * check if key is a duplicate. table_lookup() will return the latest
 * added value for a duplicate key. table_remove() will remove all
 * duplicates for a given key.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{


	if(dlist_is_empty(t->entries))
	{
		struct table_entry *first_entry = malloc(sizeof(struct table_entry));
		first_entry->key = key;
		first_entry->value = value;
		dlist_insert(t->entries, first_entry, dlist_first(t->entries)); // If the table is empty, insert the given key and value at first position
		return;
	}

	dlist_pos pos = dlist_first(t->entries);
	while (!dlist_is_end(t->entries, pos))
	{
		struct table_entry *current_entry = dlist_inspect(t->entries, pos);
		if (t->key_cmp_func(current_entry->key, key) > 0) // The given key is smaller than the first (CURRENT??) key in the list
		{
			struct table_entry *smallest_entry = malloc(sizeof(struct table_entry));
			smallest_entry->key = key;
			smallest_entry->value = value;
			dlist_insert(t->entries, smallest_entry, dlist_first(t->entries));
			return;
		}
		if(t->key_cmp_func(current_entry->key, key) == 0)
		{
			if(t->key_free_func != NULL) // Handles doubles, overwrites the old value and replace it with the new
			{
				t->key_free_func(current_entry->key);
			}

			if(t->value_free_func != NULL)
			{
				t->value_free_func(current_entry->value);
			}
			current_entry->key = key;
			current_entry->value = value;
			return;
		}
		
		pos = dlist_next(t->entries, pos);
	}

	/* If the key is greater than all the keys already in the list, insert it at the end */
	
	dlist_pos high_limit = dlist_first(t->entries);
	while (!dlist_is_end(t->entries, high_limit))
	{
		high_limit = dlist_next(t->entries, high_limit);
	}

	struct table_entry *entry = malloc(sizeof(struct table_entry));
	entry->key = key;
	entry->value = value;
	dlist_insert(t->entries, entry, high_limit); // Insert value at the newly found end of the list

}

/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table. If the table contains duplicate keys,
 * the value that was latest inserted will be returned.
 */
void *table_lookup(const table *t, const void *key)
{
	// Iterate over the list. Return first match.

	dlist_pos pos = dlist_first(t->entries);

	while (!dlist_is_end(t->entries, pos)) 
	{
		// Inspect the table entry
		struct table_entry *entry = dlist_inspect(t->entries, pos);
		// Check if the entry key matches the search key.
		if (t->key_cmp_func(entry->key, key) == 0) 
		{
			// If yes, return the corresponding value pointer.
			return entry->value;
		}
		// Continue with the next position.
		pos = dlist_next(t->entries, pos);
	}
	// No match found. Return NULL.
	return NULL;
}

/**
 * table_choose_key() - Return an arbitrary key.
 * @t: Table to inspect.
 *
 * Return an arbitrary key stored in the table. Can be used together
 * with table_remove() to deconstruct the table. Undefined for an
 * empty table.
 *
 * Returns: An arbitrary key stored in the table.
 */
void *table_choose_key(const table *t)
{
	// Return first key value.
	dlist_pos pos = dlist_first(t->entries);
	struct table_entry *entry = dlist_inspect(t->entries, pos);

	return entry->key;
}

/**
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Any matching duplicates will be removed. Will call any free
 * functions set for keys/values. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
	// Will be set if we need to delay a free.
	void *deferred_ptr = NULL;

	// Start at beginning of the list.
	dlist_pos pos = dlist_first(t->entries);

	// Iterate over the list. Remove any entries with matching keys.
	while (!dlist_is_end(t->entries, pos)) 
	{
		// Inspect the table entry
		struct table_entry *entry = dlist_inspect(t->entries, pos);

		// Compare the supplied key with the key of this entry.
		if (t->key_cmp_func(entry->key, key) == 0) 
		{
			// If we have a match, call free on the key
			// and/or value if given the responsiblity
			if (t->key_free_func != NULL) 
			{
				if (entry->key == key)
		 		{
					// The given key points to the same
					// memory as entry->key. Freeing here
					// would trigger a memory error in the
					// next iteration. Instead, defer free
					// of this pointer to the very end.
					deferred_ptr = entry->key;
				} else 
				{
					t->key_free_func(entry->key);
				}
			}
			if (t->value_free_func != NULL) 
			{
				t->value_free_func(entry->value);
			}
			// Remove the list element itself.
			pos = dlist_remove(t->entries, pos);
			// Deallocate the table entry structure.
			free(entry);
		} else 
		{
			// No match, move on to next element in the list.
			pos = dlist_next(t->entries, pos);
		}
	}
	if (deferred_ptr != NULL) 
	{
		// Take care of the delayed free.
		t->key_free_func(deferred_ptr);
	}
}

/*
 * table_kill() - Destroy a table.
 * @table: Table to destroy.
 *
 * Return all dynamic memory used by the table and its elements. If a
 * free_func was registered for keys and/or values at table creation,
 * it is called each element to free any user-allocated memory
 * occupied by the element values.
 *
 * Returns: Nothing.
 */
void table_kill(table *t)
{
	// Iterate over the list. Destroy all elements.
	dlist_pos pos = dlist_first(t->entries);

	while (!dlist_is_end(t->entries, pos)) 
	{
		// Inspect the key/value pair.
		struct table_entry *entry = dlist_inspect(t->entries, pos);
		// Free key and/or value if given the authority to do so.
		if (t->key_free_func != NULL) 
		{
			t->key_free_func(entry->key);
		}
		if (t->value_free_func != NULL) 
		{
			t->value_free_func(entry->value);
		}
		// Move on to next element.
		pos = dlist_next(t->entries, pos);
		// Deallocate the table entry structure.
		free(entry);
	}

	// Kill what's left of the list...
	dlist_kill(t->entries);
	// ...and the table.
	free(t);
}


/*



 * table_print() - Print the given table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the key/value pairs in the table and prints them.
 * Will print all stored elements, including duplicates.
 *
 * Returns: Nothing.
 
void table_print(const table *t, inspect_callback_pair print_func)
{
	// Iterate over all elements. Call print_func on keys/values.
	dlist_pos pos = dlist_first(t->entries);

	while (!dlist_is_end(t->entries, pos)) {
		struct table_entry *e = dlist_inspect(t->entries, pos);
		// Call print_func
		print_func(e->key, e->value);
		pos = dlist_next(t->entries, pos);
	}
}

static void print_int_string_pair(const void *key, const void *value)
{
	const int *k=key;
	const char *s=value;
	printf("[%d, %s]\n", *k, s);
}

// Compare two keys (int *).
static int compare_ints(const void *k1, const void *k2)
{
	int key1 = *(int *)k1;
	int key2 = *(int *)k2;

	if ( key1 == key2 )
		return 0;
	if ( key1 < key2 )
		return -1;
	return 1;
}


int main()
{
    table *t = table_empty(compare_ints, free, free);
    
    int *v = malloc(sizeof(*v));
    *v = 3; // Corrected assignment
    char *s = malloc(sizeof(*s) * 5); // Allocating memory for a string "NILS" (including null-terminator)
    strcpy(s, "NILS"); // Copying "NILS" into the allocated memory
    table_insert(t, v, s);

	int *v2 = malloc(sizeof(*v2));
    *v2 = 1; // Corrected assignment
    char *s2 = malloc(sizeof(*s2) * 5); // Allocating memory for a string "NILS" (including null-terminator)
    strcpy(s2, "PILS"); // Copying "NILS" into the allocated memory
    table_insert(t, v2, s2);

    table_print(t, print_int_string_pair);
    return 0;
}

*/