#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <table.h>

/*
 * Minimum working example for table.c. Inserts 4 key-value pairs into
 * a table, including one duplicate. Makes two lookups and prints the
 * result. The responsibility to deallocate the key-value pairs IS
 * handed over to the table. Thus, no key-value pointers need to be
 * stored outside the table.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *	    Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-02-06: First public version.
 *   v1.01 2019-03-05: Improved docs.
 *   v1.1  2023-01-14: Added printouts at start/end of main.
 */

#define VERSION "v1.1"
#define VERSION_DATE "2023-01-14"

// Create a dynamic copy of the string str.
static char* make_string_copy(const char *str)
{
	char *copy = calloc(strlen(str) + 1, sizeof(char));
	strcpy(copy, str);
	return copy;
}

// Interpret the supplied key and value pointers and print their content.
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

// Free a key pointer.
void free_key_ptr(void *p)
{
	// Convert the incoming void * to an int * purely to be able
	// to debug the deallocation.
	int *key=p;
	free(key);
}

// Free a value pointer.
void free_value_ptr(void *p)
{
	// Convert the incoming void * to a char * purely to be able
	// to debug the deallocation.
	char *value=p;
	free(value);
}

int main(void)
{
	printf("%s, %s %s: Create (integer, string) table without custom memfreehandlers.\n",
	       __FILE__, VERSION, VERSION_DATE);
	printf("Code base version %s.\n\n", CODE_BASE_VERSION);

	// Delegate the deallocation responsibility to the table.
	table *t = table_empty(compare_ints, free_key_ptr, free_value_ptr);

	int *key;
	char *value;

	key = malloc(sizeof(int));
	*key = 90187;
	value = make_string_copy("Umea");
	table_insert(t, key, value);

	key = malloc(sizeof(int));
	*key = 90184;
	value = make_string_copy("Umea");
	table_insert(t, key, value);

	key = malloc(sizeof(int));
	*key = 98185;
	value = make_string_copy("Kiruna");
	table_insert(t, key, value);

	printf("Table after inserting 3 pairs:\n");
	table_print(t, print_int_string_pair);

	int v=90187;
	const char *s=table_lookup(t,&v);
	printf("Lookup of postal code %d: %s.\n",v,s);

	key = malloc(sizeof(int));
	*key = 90187;
	value = make_string_copy("Umea (Universitet)");
	table_insert(t, key, value);

	printf("Table after adding a duplicate:\n");
	table_print(t, print_int_string_pair);

	v=90187;
	s=table_lookup(t,&v);
	printf("Lookup of postal code %d: %s.\n",v,s);

	// Kill what is left of the table.
	table_kill(t);

	printf("\nNormal exit.\n\n");
	return 0;
}
