#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <table.h>

/*
 * Minimum working example for table.c. Inserts 4 key-value pairs into
 * a table, including one duplicate. Makes two lookups and prints the
 * result. The responsibility to deallocate the key-value pairs is NOT
 * handed over to the table. Thus, all pointers must be stored outside
 * the table.
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

int main(void)
{
	printf("%s, %s %s: Create (integer, string) table without any memfreehandler.\n",
	       __FILE__, VERSION, VERSION_DATE);
	printf("Code base version %s.\n\n", CODE_BASE_VERSION);

	// Keep track of the key-value pairs we allocate.
	int *key[4];
	char *value[4];

	table *t = table_empty(compare_ints, NULL, NULL);

	key[0] = malloc(sizeof(int));
	*key[0] = 90187;
	value[0] = make_string_copy("Umea");
	table_insert(t, key[0], value[0]);

	key[1] = malloc(sizeof(int));
	*key[1] = 90184;
	value[1] = make_string_copy("Umea");
	table_insert(t, key[1], value[1]);

	key[2] = malloc(sizeof(int));
	*key[2] = 98185;
	value[2] = make_string_copy("Kiruna");
	table_insert(t, key[2], value[2]);

	printf("Table after inserting 3 pairs:\n");
	table_print(t, print_int_string_pair);

	int v=90187;
	const char *s=table_lookup(t,&v);
	printf("Lookup of postal code %d: %s.\n",v,s);

	key[3] = malloc(sizeof(int));
	*key[3] = 90187;
	value[3] = make_string_copy("Umea (Universitet)");
	table_insert(t, key[3], value[3]);

	printf("Table after adding a duplicate:\n");
	table_print(t, print_int_string_pair);

	v=90187;
	s=table_lookup(t,&v);
	printf("Lookup of postal code %d: %s.\n",v,s);

	// Kill what is left of the table.
	table_kill(t);

	// Free key/value pairs that we put in the table.
	for (int i=0; i<sizeof(key)/sizeof(key[0]); i++) {
		free(key[i]);
		free(value[i]);
	}

	printf("\nNormal exit.\n\n");
	return 0;
}
