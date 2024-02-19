#include <stdlib.h>
#include <stdio.h>
#include <table.h>
#include <dlist.h>

/*
 * This code provides functionality for creating and manipulating a generic
 * table data structure. The table is implemented as sorted list.
 * 
 * Duplicates are handled by insert.
 *
 * Authors: Sebastian Gabrielsson ()
 *	    Nils Sjölund ()
 *
 * Version information:
 * 
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


/**
 * table_empty() - Create an empty table.
 * @key_cmp_func: A pointer to a function to be used to compare keys.
 * @key_free_func: A pointer to a function (or NULL) to be called to
 *		   de-allocate memory for keys on remove/kill.
 * @value_free_func: A pointer to a function (or NULL) to be called to
 *		     de-allocate memory for values on remove/kill.
 *
 * Returns: Pointer to the newly created table.
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
 * @key: A pointer to value of the key.
 * @value: A pointer to value stored at said key.
 *
 * Insert the key/value pair into the table. The function tests for
 * duplicates. 
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
	struct table_entry *entry = malloc(sizeof(struct table_entry));
	entry->key = key;
	entry->value = value;

	if(dlist_is_empty(t->entries))
	{
		dlist_insert(t->entries, entry, dlist_first(t->entries)); // If the table is empty, insert the given key and value at first position
		return;
	}

	dlist_pos pos = dlist_first(t->entries);
	while (!dlist_is_end(t->entries, pos))
	{
		struct table_entry *current_entry = dlist_inspect(t->entries, pos);
		if (t->key_cmp_func(current_entry->key, key) > 0) // The given key is smaller than the first key in the list
		{
			dlist_insert(t->entries, entry, pos);
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
			free(entry); // Free the new entry since you dont need it
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
	
	dlist_insert(t->entries, entry, high_limit); // Insert value at the newly found end of the list

}

/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table.
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
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Removes any key and value from the table. The function also
 * calls to any free functions for keys/values. Does nothing if
 * key is not found.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
	// Start at beginning of the list.
	dlist_pos pos = dlist_first(t->entries);

	// Iterate over the list. Remove any entries with matching keys.
	while (!dlist_is_end(t->entries, pos)) 
	{
		// Inspect the table entry
		struct table_entry *tmp_entry = dlist_inspect(t->entries, pos);

		// Compare the supplied key with the key of this entry.
		if (t->key_cmp_func(tmp_entry->key, key) == 0) 
		{
			if (t->key_free_func != NULL) 
			{
				t->key_free_func(tmp_entry->key);
			}
			if (t->value_free_func != NULL) 
			{
				t->value_free_func(tmp_entry->value);
			}
			// Deallocate the table entry structure.
			free(tmp_entry);
			// Remove the list element itself.
			dlist_remove(t->entries, pos);

			return;
		} else 
		{
			// No match, move on to next element in the list.
			pos = dlist_next(t->entries, pos);
		}
	}
}

/**
 * table_kill() - Destroy a table.
 * @table: Table to destroy.
 *
 * Iterates over the list, kills all elements
 * and frees all memory allocated.
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

		// Deallocate the table entry structure.
		free(entry);

		// Move on to next element.
		pos = dlist_next(t->entries, pos);
	}

	dlist_kill(t->entries);
	free(t);
}

/**
 * table_print() - Print the given table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the key/value pairs in the table and prints them.
 * Will print all stored elements, including duplicates.
 *
 * Returns: Nothing.
 */
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