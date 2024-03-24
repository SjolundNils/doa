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
 *	v1.0 2024-02-20 first public version
 *	v1.1 2024-03-03 fixed comments
 */

// ===========INTERNAL DATA TYPES============

struct table
{
	array_1d *entries; // Array for storing value/key pairs
	compare_function *key_cmp_func; // Function for comparing keys
	free_function key_free_func; // Function for freeing key
	free_function value_free_func; // Function for freeing value
	int size; // Size of table
};

struct table_entry
{
	void *key; // Pointer to key
	void *value; // Pointer to value
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
	// If the table is empty, return NULL
	if (table_is_empty(t)){		
		return NULL;
	}

	// Otherwise, return the key of the first entry
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
 * If no match is found, shift all entries with a larger key one step "up",
 * insert the key/value at the correct index
 * 
 *
 * Returns: Nothing.
 */
void table_insert(table *t, void *key, void *value)
{
	int low = 0; // Low limit for binary seach
	int high = t->size - 1; // High limit for binary seach
	int mid = low + (high - low) / 2; // Position for middle value to inspect
	int insert_index = t->size; // Defaults to last open space
	
	while (low <= high) 
	{
		mid = low + (high - low) / 2; // Update middle position
		struct table_entry *mid_entry = array_1d_inspect_value(t->entries, mid); // Entry to inspect
		
		// Compare the supplied key with the key of this entry.
		if (t->key_cmp_func(mid_entry->key, key) == 0)
		{
			// If we have a match, call free on the key
			// and/or value if given the responsiblity
			if (t->value_free_func != NULL)
			{
				t->value_free_func(mid_entry->value);
			}
			if (t->key_free_func != NULL)
			{
				// Free the old key since the new key is already allocated.
				t->key_free_func(mid_entry->key); 
			}
			// Set the duplicate key/value pair to the inserted values
			mid_entry->key = key;
			mid_entry->value = value;
			return;
		}
		// If supplied key is smaller than the key in the middle
		else if (t->key_cmp_func(mid_entry->key, key) > 0) 
		{
			insert_index = mid; // Set insert_index to mid since it can't be t->size
			high = mid - 1; // Update high limit
		}
		else // If supplied key is larger than the key in the middle
		{
			// insert_index could still be t->size
			low = mid + 1; // Update low limit
		}
	}

	// If no match is found, allocate memory for a new entry
	struct table_entry *entry = malloc(sizeof(struct table_entry));
	entry->value = value;
	entry->key = key;

	// Shift elements to make space for the new entry.
	for (int i = t->size; i > insert_index; i--)
	{
		struct table_entry *shifted_entry = array_1d_inspect_value(t->entries, i - 1);
		array_1d_set_value(t->entries, shifted_entry, i);
	}

	// Insert the new entry at the correct position.
	array_1d_set_value(t->entries, entry, insert_index);

	// Increase the table size.
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
	int low = 0; // Low limit for binary seach
	int high = t->size - 1; // High limit for binary seach
	int mid = low + high - low / 2; // Position for middle value to inspect

	while (low <= high)
	{
		mid = low + (high - low) / 2; // Update middle position
		struct table_entry *entry = array_1d_inspect_value(t->entries, mid);
		
		// Compare the supplied key with the key of this entry.
		if (t->key_cmp_func(entry->key, key) == 0)
		{
			// Supplied key is found, return value stored at the key 
			return entry->value; 
		}
		
		// If supplied key is smaller than the key in the middle
		else if (t->key_cmp_func(entry->key, key) > 0)
		{
			high = mid - 1; // Update high limit
		}
		else // If supplied key is larger than the key in the middle
		{
			low = mid + 1; // Update low limit
		}
	}
	//If no match is found, return NULL
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
	int low = 0; // Low limit for binary seach
	int high = t->size - 1; // High limit for binary seach
	int mid = low + (high - low) / 2; // Position for middle value to inspect
	
	// Store the removed elements index (-1 if no element was removed)
	int removed_index = -1;

	while (low <= high)
	{
		mid = low + (high - low) / 2;
		struct table_entry *mid_entry = array_1d_inspect_value(t->entries, mid);
		
		// Compare the supplied key with the key of this entry.
		if (t->key_cmp_func(mid_entry->key, key) == 0)
		{
			// If we have a match, call free on the key
			// and/or value if given the responsiblity
			if (t->value_free_func != NULL)
			{
				// Free the value
				t->value_free_func(mid_entry->value);
			}
			if (t->key_free_func != NULL)
			{
				// Free the key
				t->key_free_func(mid_entry->key);
			}
			free(mid_entry);
			// Save the index, for shifting elements
			removed_index = mid;
			break;
		}
		
		// If supplied key is smaller than the key in the middle
		else if (t->key_cmp_func(mid_entry->key, key) > 0)
		{
			high = mid - 1; // Update high limit
		}
		else // If supplied key is larger than the key in the middle
		{
			low = mid + 1; //Update low limit
		}
	}

	// If no element was removed, return
	if (removed_index == -1)
	{
		return;
	}

	// Shift all larger elements one step back and decrease size
	for (int i = removed_index + 1; i < t->size; i++)
	{
		struct table_entry *shifted_entry = array_1d_inspect_value(t->entries, i);
		array_1d_set_value(t->entries, shifted_entry, i - 1);
	}
	t->size--; // Decrease table-size
}

/**
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