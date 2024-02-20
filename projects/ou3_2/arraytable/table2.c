#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <array_1d.h>
#include <table.h>
#include <string.h>

#define MAXSIZE 80000

/*
 * Implementation of a generic table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Duplicates are handled by insert.
 *
 * Authors:
 *	Nils Sjölund (id23nsd@cs.umu.se)
 * 	Sebastian Gabrielsson (id23sgn@cs.umu.se)
 *
 * Based on earlier code by: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2024-02-20 first public version
 *
 */

// ===========INTERNAL DATA TYPES============

struct table
{
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
	int size;
};

struct table_entry
{
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
	//  Allocate the table header.
	table *t = calloc(1, sizeof(struct table));

	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;

	// Set the table-size to 0
	//(used in table_is_empty() and as an index in the array)
	t->size = 0;

	// Create the array to hold the table_entry-ies.
	t->entries = array_1d_create(0, MAXSIZE - 1, NULL);
	return t;
}

/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Returns: True if the table-size is 0, false otherwise.
 */
bool table_is_empty(const table *t)
{
	return (t->size == 0);
}

/**
 * table_choose_key() - Return an arbitrary key.
 * @t: Table to inspect.
 *
 * Return an arbitrary key stored in the table. 
 * Undefined for an empty table.
 *
 * Returns: An arbitrary key stored in the table.
 */
void *table_choose_key(const table *t)
{
	if (table_is_empty(t)){		
		return NULL;
	}

	struct table_entry *entry = array_1d_inspect_value(t->entries, 0);
	return entry->key;
}

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. Start by running a test to
 * check if key is a duplicate.
 *
 * If match a is found, Deallocate the
 * duplicate and insert a new key/value pair with a new value
 *
 * If no match is found, insert the key/value pair at the
 * "end" of the array and increase the table size by 1.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
	// Iterate trough the table and search for duplicate.
	for (int i = 0; i < t->size; i++)
	{
		struct table_entry *inspection_entry = array_1d_inspect_value(t->entries, i);
		if (t->key_cmp_func(inspection_entry->key, key) == 0) // If a duplicate is found
		{
			if (t->key_free_func != NULL)
			{
				t->key_free_func(inspection_entry->key); // Free the old key since the new key is already allocated.
			}
			if (t->value_free_func != NULL)
			{
				t->value_free_func(inspection_entry->value); // Free the "old" value
			}
			// Set the duplicate key/value pair to the inserted values
			inspection_entry->value = value;
			inspection_entry->key = key;
			return;
		}
	}
	// If no match is found, insert the entry to the "end" of the table and increase the table size
	struct table_entry *entry = malloc(sizeof(struct table_entry));
	entry->value = value;
	entry->key = key;
	array_1d_set_value(t->entries, entry, t->size);
	t->size++;
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
	// Iterate trough the array and search for a match
	for (int i = 0; i < t->size; i++)
	{
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);
		if (t->key_cmp_func(entry->key, key) == 0)
		{
			return entry->value;
		}
	}
	return NULL;
}

/**
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Will call any free functions set for keys/values. 
 * Does nothing if key is not found in the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
	// Iterate trough the array and search for a match
	for (int i = 0; i < t->size; i++)
	{
		struct table_entry *inspection_entry = array_1d_inspect_value(t->entries, i);
		// If a match is found
		if (t->key_cmp_func(inspection_entry->key, key) == 0)
		{
			// Free key and/or value if given the authority to do so.
			if (t->key_free_func != NULL)
			{
				t->key_free_func(inspection_entry->key);
			}
			if (t->value_free_func != NULL)
			{
				t->value_free_func(inspection_entry->value);
			}

			// Free the matching entry and insert the last entry in its place
			free(inspection_entry);
			struct table_entry *last_entry = array_1d_inspect_value(t->entries, t->size - 1);
			array_1d_set_value(t->entries, last_entry, i);

			// Decrease the table-size
			t->size--;
			return;
		}
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
	//  Iterate over the array. Destroy all elements.
	for (int i = 0; i < t->size; i++)
	{
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);
		if (entry != NULL)
		{
			// Free key and/or value if given the authority to do so.
			if (t->key_free_func != NULL)
			{
				t->key_free_func(entry->key);
			}
			if (t->value_free_func != NULL)
			{
				t->value_free_func(entry->value);
			}
			// Free the entry-pointer itself
			free(entry);
		}
	}
	// Kill whats left of the array
	array_1d_kill(t->entries);
	// And free the table-header
	free(t);
}

/**
 * table_print() - Print the given table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the key/value pairs in the table and prints them.
 *
 * Returns: Nothing.
 */
void table_print(const table *t, inspect_callback_pair print_func)
{
	// Iterate trough the table
	for (int i = 0; i < t->size; i++)
	{
		struct table_entry *e = array_1d_inspect_value(t->entries, i);
		// Call print_func
		print_func(e->key, e->value);
	}
}