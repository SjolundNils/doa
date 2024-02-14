#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <array_1d.h>
#include <table.h>
#include <string.h>

#define MAXSIZE 40000

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
	// printf("RUNNING TABLE_EMPTY\n");
	//  Allocate the table header.
	table *t = calloc(1, sizeof(struct table));
	// Create the list to hold the table_entry-ies.

	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;
	t->size = 0;
	t->entries = array_1d_create(0, MAXSIZE - 1, NULL);
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
	// printf("RUNNING TABLE_IS_EMPTY\n");
	return (t->size == 0);
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
	// printf("RUNNING TABLE_INSERT\n");
	//  Allocate the key/value structure.
	for (int i = 0; i < t->size; i++)
	{
		struct table_entry *inspection_entry = array_1d_inspect_value(t->entries, i);
		if (t->key_cmp_func(inspection_entry->key, key) == 0)
		{
			if (t->key_free_func != NULL)
			{
				t->key_free_func(inspection_entry->key);
			}
			if (t->value_free_func != NULL)
			{
				t->value_free_func(inspection_entry->value);
			}
			free(inspection_entry);

			struct table_entry *replacement_entry = malloc(sizeof(struct table_entry));
			replacement_entry->value = value;
			replacement_entry->key = key;
			array_1d_set_value(t->entries, replacement_entry, t->size);
			return;
		}
	}

	struct table_entry *entry = malloc(sizeof(struct table_entry));
	entry->value = value;
	entry->key = key;
	// Check if memory allocation was successful
	if (entry == NULL)
	{
		// Handle allocation failure
		return;
	}

	array_1d_set_value(t->entries, entry, t->size);
	t->size++;
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
	// printf("RUNNING TABLE_LOOKUP\n");
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
 * Any matching duplicates will be removed. Will call any free
 * functions set for keys/values. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 */
void table_remove(table *t, const void *key)
{
	// printf("RUNNING TABLE_REMOVE\n");
	for (int i = 0; i < t->size; i++)
	{

		struct table_entry *inspection_entry = array_1d_inspect_value(t->entries, i);
		if (inspection_entry != NULL && t->key_cmp_func(inspection_entry->key, key) == 0)
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

			free(inspection_entry);
			struct table_entry *final_entry = array_1d_inspect_value(t->entries, t->size - 1);
			array_1d_set_value(t->entries, final_entry, i);

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
	// printf("RUNNING TABLE_KILL\n");
	//  Iterate over the list. Destroy all elements.
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
			free(entry);
		}
	}
	array_1d_kill(t->entries);
	free(t);
	// printf("TABLE_KILL SUCCESSFULL\n");
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
	// printf("RUNNING TABLE_PRINT");
	for (int i = 0; i < t->size; i++)
	{
		struct table_entry *e = array_1d_inspect_value(t->entries, i);
		// Call print_func
		print_func(e->key, e->value);
	}
}