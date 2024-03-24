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
 * Authors: Sebastian Gabrielsson (id23sgn@cs.umu.se)
 *	   	 	Nils Sjölund (id23nsd@cs.umu.se)
 *
 * Based on earlier code by: Niclas Borlin (niclas@cs.umu.se)
 * 							 Adam Dahlgren Lindstrom (dali@cs.umu.se)
 * 
 * Version information:
 * 	v1.0 2024-02-14 first public version
 *  v1.1 2024-03-03 fixed comments
 */

// ===========INTERNAL DATA TYPES============

struct table {
	dlist *entries; // List for storing key/value pairs
	compare_function *key_cmp_func; // Function for comparing keys
	free_function key_free_func; // Function for freeing key
	free_function value_free_func; // Function for freeing val
};

struct table_entry {
	void *key; // Pointer to key
	void *value; // Pointer to key
};


/**
 * table_empty() - Create an empty table.
 * @key_cmp_func: A pointer to a function to be used for key comparison
 * @key_free_func: A pointer to a function thats de-alocates memory for keys.
 * 				Can also call the function with NULL.
 * @value_free_func: A pointer to a function that de-alocates memory for values
 * 				stored at a key. Can be called with NULL.
 *
 * Returns: Pointer to the newly created table.
 */
table *table_empty(compare_function *key_cmp_func,
		   free_function key_free_func,
		   free_function value_free_func)
{
	// Allocate the table header.
	table *t = calloc(1, sizeof(table));
	
	// Create the list to hold table entries
	t->entries = dlist_empty(NULL);
	
	// Store the key compare function and free functions in the struct
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;

	return t;
}

/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Returns: True if the table contains a key/value, otherwise false.
 */
bool table_is_empty(const table *t)
{
	return dlist_is_empty(t->entries);
}

/**
 * table_insert() - Inserts a key paired with a value into the table.
 * @table: Table to add to.
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
	// Allocate the key/value structure.
	struct table_entry *entry = malloc(sizeof(struct table_entry));
	
	// Set the pointers and insert first in the list. This will
	// cause table_lookup() to find the latest added value.
	entry->key = key;
	entry->value = value;

	// If the table is empty, 
	// insert the given key and value at first position
	if(dlist_is_empty(t->entries))
	{
		dlist_insert(t->entries, entry, dlist_first(t->entries));
		return;
	}

	dlist_pos pos = dlist_first(t->entries);
	while (!dlist_is_end(t->entries, pos))
	{
		struct table_entry *current_entry = dlist_inspect(t->entries, pos);
		if (t->key_cmp_func(current_entry->key, key) > 0) // The given key is smaller than the first key in the list
		{
			
			dlist_insert(t->entries, entry, dlist_first(t->entries));
			return;
		}
		
		// If we have a match, call free on the key
		// and/or value if given the responsiblity
		if(t->key_cmp_func(current_entry->key, key) == 0)
		{
			if(t->key_free_func != NULL) // Handles doubles, overwrites the old value and replace it with the new
			{
				t->key_free_func(current_entry->key); // If free was called upon when the table was made, free the key
			}

			if(t->value_free_func != NULL)
			{
				t->value_free_func(current_entry->value); // Same here as with the key, but instead free the value
			}
			current_entry->key = key;
			current_entry->value = value;
			free(entry);
			return;
		}
		
		pos = dlist_next(t->entries, pos);
	}

	// If the key is greater than all the keys already in the list, insert it at the end
	
	dlist_pos high_limit = dlist_first(t->entries);
	while (!dlist_is_end(t->entries, high_limit)) // Finds the end of the list
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
 * Returns: The value stored at given key, return NULL if the
 * 		key is not found.
 */
void *table_lookup(const table *t, const void *key)
{
	dlist_pos pos = dlist_first(t->entries);

	while (!dlist_is_end(t->entries, pos)) // Iterate over the list 
	{
		// Inspect the table entry
		struct table_entry *entry = dlist_inspect(t->entries, pos);
		
		// Check if the entry key matches the searched key.
		if (t->key_cmp_func(entry->key, key) == 0) 
		{
			// If they match, return pointer to the value stored at said key
			return entry->value;
		}
		// If they dont, continue with the next position.
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
		struct table_entry *entry = dlist_inspect(t->entries, pos);

		// Compare the searched key with key at current position
		if (t->key_cmp_func(entry->key, key) == 0) 
		{
			if (t->key_free_func != NULL) // If free was called upon when the table was made, free the key
			{
				t->key_free_func(entry->key);
			}
			if (t->value_free_func != NULL) // Same here as with the key, but instead free the value
			{
				t->value_free_func(entry->value);
			}
			// Apart from freeing the element, we need to remove it as well
			pos = dlist_remove(t->entries, pos);
			
			// Deallocate the temporary entry
			free(entry);
			return;
		} else 
		{
			// No match, keep moving through the list
			pos = dlist_next(t->entries, pos);
		}
	}
}

/**
 * table_kill() - Destroy an entire table
 * @table: Table to destroy.
 *
 * Iterates over the list, kills all elements
 * and frees all memory allocated.
 *
 * Returns: Nothing.
 */
void table_kill(table *t)
{
	dlist_pos pos = dlist_first(t->entries);

	while (!dlist_is_end(t->entries, pos)) 
	{
		struct table_entry *entry = dlist_inspect(t->entries, pos);

		if (t->key_free_func != NULL) 
		{
			t->key_free_func(entry->key);
		}
		if (t->value_free_func != NULL) 
		{
			t->value_free_func(entry->value);
		}

		pos = dlist_next(t->entries, pos);

		free(entry);
	}

	dlist_kill(t->entries);
	free(t);
}

/**
 * table_print() - Print the table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the entries in the table and prints them.
 *
 * Returns: Nothing.
 */
void table_print(const table *t, inspect_callback_pair print_func)
{
	dlist_pos pos = dlist_first(t->entries);

	while (!dlist_is_end(t->entries, pos)) {
		struct table_entry *e = dlist_inspect(t->entries, pos);
		// Call print function
		print_func(e->key, e->value);
		pos = dlist_next(t->entries, pos);
	}
}