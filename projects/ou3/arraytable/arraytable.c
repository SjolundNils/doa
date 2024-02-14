#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <array_1d.h>
#include <table.h>
#include <string.h>

#define MAXSIZE 80000

struct table
{
	array_1d *entries;
	int size;

	/*------Osäker------*/
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
};

struct table_entry
{
	void *key;
	void *value;
};

table *table_empty(compare_function *key_cmp_func,
				   free_function key_free_func,
				   free_function value_free_func)
{
	table *t = calloc(1, sizeof(struct table));			// Allokera minne
	t->entries = array_1d_create(0, MAXSIZE - 1, free); // Skapa en array som kommer att bli tabellens entries. Låt den gå från 0 till MAXSIZE - 1
	t->size = 0;

	t->key_cmp_func = key_cmp_func;		  // Sätt tabelens compare_function till den inmatade funktionen
	t->key_free_func = key_free_func;	  // Sätt tabelens key_free_function till den inmatade funktionen
	t->value_free_func = value_free_func; // Sätt tabelens value_free_function till den inmatade funktionen

	/*--------------------------------------------------------------------------------*/
	return t;
}

void table_insert(table *t, void *key, void *value)
{
	printf("Running table_insert\n");
	printf("Input table size: %d\n", t->size);
	// Allokera minne för en post
	// struct table_entry *entry = malloc(sizeof(struct table_entry));
	struct table_entry *entry = malloc(sizeof(struct table_entry));

	printf("Allocation successfull\n");

	if (t->size != 0)
	{
		for (int lookup_index = 0; lookup_index < t->size; lookup_index++)
		{
			struct table_entry *tmp_entry = array_1d_inspect_value(t->entries, lookup_index);
			printf("For loop iteration: %d\n", lookup_index);
			// if(entry->key == key)

			printf("Before if(59)\n");
			if (t->key_cmp_func(tmp_entry->key, key) == 0)
			{
				printf("Entered if(59)\n");
				entry->value = value;
				entry->key = key;
				array_1d_set_value(t->entries, entry, lookup_index);
				return;
			}
			printf("After if(59)\n");
		}
	}
		printf("t->size != 0 returned false\n");
		entry->key = key;
		entry->value = value;
		/*-------------------------------------------------------------------------------------*/
		array_1d_set_value(t->entries, entry, t->size);
		t->size++;
		printf("array_set_value sucessfull\n");
}

bool table_is_empty(const table *t)
{
	// return array_1d_has_value(t->entries, 0); //Kollar om index 0 har ett värde, VIKTIGT se till att alltid fylla arrayen från 0 och uppåt.
	return t->size == 0;
}

void table_remove(table *t, const void *key)
{
	printf("Runninng table remove...\n");
	for (int lookup_index = 0; lookup_index <= t->size; lookup_index++)
	{
		printf("For loop entered\n");
		// Inspect the table entry, store it in *entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, lookup_index);
		// Check if the entry key matches the search key.
		printf("Before if(97)\n");
		if (entry != NULL && t->key_cmp_func(entry->key, key) == 0)
		{
			printf("Entered if(97)\n");
			// Markera posten som tom genom att tilldela NULL till dess värden.
			array_1d_set_value(t->entries, NULL, lookup_index);

			struct table_entry *last_value = array_1d_inspect_value(t->entries, t->size-1);
			array_1d_set_value(t->entries, last_value, lookup_index);
			array_1d_set_value(t->entries, NULL, t->size-1);
			// Minska antalet poster i tabellen, om nödvändigt
			t->size--;
			printf("before break\n");
			break; // Vi har hittat och markerat posten som tom, så vi kan avbryta loopen
		}
	}
}

void table_kill(table *t)
{
	// Free all memory associated with entries
	for (int lookup_index = 0; lookup_index <= MAXSIZE; lookup_index++)
	{
		struct table_entry *entry = array_1d_inspect_value(t->entries, lookup_index);
		if (entry != NULL)
		{
			if (t->key_free_func != NULL)
			{
				// printf("\n\n\n\nVI TOG OSS IN I IF-SATSEN\n");
				// array_1d_set_value(entry->key, NULL, lookup_index);
				// printf("if-satsen rad 1 - OK\n");
				t->key_free_func(entry->key);
				// printf("If-satsen rad 2 - OK");
			}
			if (t->value_free_func != NULL)
			{
				// printf("VI TOG OSS IN I DENNA IF SATS YIPPIE\n");
				// array_1d_set_value(entry->value, NULL, lookup_index);
				t->value_free_func(entry->value);
			}
		}
	}
	array_1d_kill(t->entries); // Free the array memory
	free(t);
}

void *table_lookup(const table *t, const void *key)
{
	printf("Running table lookup...\n");
	int test = 1;
	// Iterate over the array. Return first match.
	for (int lookup_index = 0; lookup_index < t->size; lookup_index++)
	{
		printf("Entered for loop...\n");
		// Inspect the table entry store it in *entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, lookup_index);
		// Check if the entry key matches the search key.
		printf("Before if (154)\n");
		if (t->key_cmp_func(entry->key, key) == 0)
		{
			printf("Entered if (154)\n");
			// If yes, return the corresponding value pointer.
			return entry->value;
		}
		printf("For loop nummer %d\n", test);
		test++;
	}
	printf("For loop done\n");
	// No match found. Return NULL.
	return NULL;
}
/*
void struct_print_func(const void *cell) {
	const struct table_entry *entry = (const struct table_entry *)cell;
	printf("[%d].[%d]", *(int*)(entry->key), *(int*)(entry->value));
}

void table_print(const table *t, inspect_callback_pair print_func)
{
	// Iterate over all elements. Call print_func on keys/values.
	for (int i; i < t->size; i++)
	{
		struct table_entry *e = array_1d_inspect_value(t->entries, i);
		// Call print_func
		print_func(e);
	}
}

int int_compare(const void *ip1,const void *ip2)
{
		const int *n1=ip1;
		const int *n2=ip2;
		return (*n1 - *n2);
}

int string_compare(const void *ip1,const void *ip2)
{
		const char *s1=ip1;
		const char *s2=ip2;
		return strcmp(s1,s2);
}

int main(void)
{
	table *t = table_empty(NULL, NULL, NULL);


	return 0;
}

*/