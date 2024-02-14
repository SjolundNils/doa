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
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
	int size;
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
	t->entries = array_1d_create(0, MAXSIZE - 1, NULL);
	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;
	t->size = 0;
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
	return array_1d_inspect_value(t->entries, 0) == NULL;
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
	// Allocate the key/value structure.
	struct table_entry *entry = malloc(sizeof(struct table_entry));

	// Set the pointers and insert first in the list. This will
	// cause table_lookup() to find the latest added value.
	entry->key = key;
	entry->value = value;
	array_1d_set_value(t->entries, entry, t->size);
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
	for(int i = 0; i < MAXSIZE; i++) 
	{
		// Inspect the table entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);
		// Check if the entry key matches the search key.
		if (t->key_cmp_func(entry->key, key) == 0) {
			// If yes, return the corresponding value pointer.

			/*-------------*/
			array_1d_set_value(t->entries, NULL, i); //"Disconnect" the entry from t->entries        	
            return entry->value; //Return the value of the entry
			/*-------------*/
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
 
void *table_choose_key(const table *t)
{
	// Return first key value.
	struct table_entry *entry = array_1d_inspect_value(t->entries, 0);

	return entry->key;
}
*/


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
	for (int i = 0; i < MAXSIZE; i++) 
	{
		// Inspect the table entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, i);
		// Compare the supplied key with the key of this entry.
		if (t->key_cmp_func(entry->key, key) == 0) {
			// If we have a match, call free on the key
			// and/or value if given the responsiblity
			if (t->key_free_func != NULL) {
				if (entry->key == key) {
					// The given key points to the same
					// memory as entry->key. Freeing here
					// would trigger a memory error in the
					// next iteration. Instead, defer free
					// of this pointer to the very end.
					deferred_ptr = entry->key;
				} else {
				t->key_free_func(entry->key);
			}
			}
			if (t->value_free_func != NULL) {
				t->value_free_func(entry->value);
			}
			// Remove the list element itself.
			pos = dlist_remove(t->entries, pos);
			// Deallocate the table entry structure.
			free(entry);
		} else {
			// No match, move on to next element in the list.
			pos = dlist_next(t->entries, pos);
		}
	}
	if (deferred_ptr != NULL) {
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

	while (!dlist_is_end(t->entries, pos)) {
		// Inspect the key/value pair.
		struct table_entry *entry = dlist_inspect(t->entries, pos);
		// Free key and/or value if given the authority to do so.
		if (t->key_free_func != NULL) {
			t->key_free_func(entry->key);
		}
		if (t->value_free_func != NULL) {
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





///////////////////////////HÄR BÖRJAR VÅR KOD////////////////////////////////////
/*



///
void struct_print_func(const void *cell) {
    const struct table_entry *entry = (const struct table_entry *)cell;
    printf("[%d].[%d]", *(int*)(entry->key), *(int*)(entry->value));
}

///

/**
 * int_compare() - Compare to integers via pointers.
 * @ip1, @ip2: Pointers to integers to be compared.
 *
 * Compares the integers that ip1 and ip2 points to.
 *
 * Returns: 0 if the integers are equal, negative if the first
 * argument is smaller, positive if the first argument is larger.
 */
int int_compare(const void *ip1,const void *ip2)
{
        const int *n1=ip1;
        const int *n2=ip2;
        return (*n1 - *n2);
}

int main(void)
{
    table *t = table_empty(int_compare, free, free);
    
    for (int i = 1; i <= 5; i++)
    {
        int *v = malloc(sizeof(*v));
        *v = i;
        table_insert(t, v, v);
    }

    printf("Listan från början\n");
    dlist_print(t->entries, struct_print_func);

    int *v = malloc(sizeof(*v));
    *v = 3;

    printf("\nLOOKUP: ");
    struct table_entry *lookup_entry = table_lookup(t, v);
    if (lookup_entry != NULL) {
		int *lookup_value = lookup_entry->value;
		int *lookup_key = lookup_entry->key;
        printf("[%d].[%d]\n\n", lookup_key, lookup_value);
    } else {
        printf("Inget värde hittades för nyckeln.\n\n");
    }
	printf("Listan efter LOOKUP:\n");
    dlist_print(t->entries, struct_print_func);

    printf("\n\n\n");
}
*/