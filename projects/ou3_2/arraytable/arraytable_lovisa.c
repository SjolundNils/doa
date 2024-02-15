#include <stdlib.h>
#include <stdio.h>

#include <table.h>
#include <array_1d.h>

#define MAXSIZE 80000

/*
 * Implementation of a generic table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Duplicates are handled by inspect and remove.
 *
 * Authors: Lovisa Nordström (id23lnm@cs.umu.se)
 *	    Anna Olsson (id23aon@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se) and Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0 2024-02-: First version to hand in
 */

struct table {
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
	int occupied_position;
};

struct table_entry {
	void *key;
	void *value;
};

// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * table_empty() - Create an empty table and sets the number of occupied positions to 0.
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
	// Create the array to hold the table_entry-ies.
	t->entries = array_1d_create(0, MAXSIZE, NULL);
	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;

	t->occupied_position = 0;

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
	return t->occupied_position == 0;
}

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. A test is performed to
 * check if key is a duplicate. If there is a duplicate, 
 * the new value will overwrite the previous value.
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
	for(int i = 0; i < t->occupied_position; i++)
	{
		struct table_entry *current_entry = array_1d_inspect_value(t->entries, i);

		// Check if the entry key matches the search key.
		if (t->key_cmp_func(current_entry->key, key) == 0) 
		{
			// Free the existing entry's memory before overwriting it.
			if (t->key_free_func != NULL)
			{
				t->key_free_func(current_entry->key);
			}

			if (t->value_free_func != NULL) {
				t->value_free_func(current_entry->value);
			}

			current_entry->key = key;
			current_entry->value = value;

			//array_1d_set_value(t->entries, current_entry, i);
			return;
		}	
	}	

	// Allocate the key/value structure.
	struct table_entry *entry = malloc(sizeof(struct table_entry));

	// Set the pointers and insert at the first free position in the array. This will
	// cause table_lookup() to find the latest added value.
	entry->key = key;
	entry->value = value;

	array_1d_set_value(t->entries, entry, t->occupied_position);
	t->occupied_position++;
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
	// Iterate over the array. Return if there is a match.

	for (int i = 0; i < t->occupied_position; i++) {

		// Inspect the table entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);

		// Check if the entry key matches the search key.
		if (t->key_cmp_func(entry->key, key) == 0) {
			// If yes, return the corresponding value pointer.
			return entry->value;
		}
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
	int first_index = array_1d_low(t->entries);
	struct table_entry *entry = array_1d_inspect_value(t->entries, first_index);

	return entry->key;
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
	for( int i = 0; i < t->occupied_position; i++) {

		// Inspect the table entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);

		if(t->key_cmp_func(entry->key, key) == 0) {

			if (t->key_free_func != NULL) {
				t->key_free_func(entry->key);
			}

			if (t->value_free_func != NULL) {
				t->value_free_func(entry->value);
			}

			free(entry);

			// Move the last entry to the current position
            struct table_entry *last_entry = array_1d_inspect_value(t->entries, t->occupied_position - 1);
            array_1d_set_value(t->entries, last_entry, i);

            // Set the last entry to NULL
            array_1d_set_value(t->entries, NULL, t->occupied_position - 1);

			t->occupied_position --;
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
	// Iterate over the table. Destroy all elements.

	for(int i = 0; i < t->occupied_position; i++) {

		// Inspect the table entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);

		// Free key and/or value if given the authority to do so.
		if (t->key_free_func != NULL) {
			t->key_free_func(entry->key);
		}

		if (t->value_free_func != NULL) {
			t->value_free_func(entry->value);
		}
		free(entry);
	}

	// Kill what's left of the array...
	array_1d_kill(t->entries);
	// ...and the table.
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
	for(int i = 0; i < t->occupied_position; i++) {
		
		struct table_entry *e = array_1d_inspect_value(t->entries, i);
		// Call print_func
		array_1d_print(e->key, e->value);

	}
}
//gcc -std=c99 -Wall -I ../../code_base/current/include/ -o tabletest-1.9 tabletest-1.9.c arraytable.c ../../code_base/current/src/array_1d/array_1d.c


