#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <array_1d.h>
#include <table.h>

#define MAXSIZE 80000

struct table
{
	array_1d *entries;
	int *size;

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

	/*------ Vi ska förmodligen implementera själv, vet inte hur han vill ha de ------*/

	t->key_cmp_func = key_cmp_func;		  // Sätt tabelens compare_function till den inmatade funktionen
	t->key_free_func = key_free_func;	  // Sätt tabelens key_free_function till den inmatade funktionen
	t->value_free_func = value_free_func; // Sätt tabelens value_free_function till den inmatade funktionen

	/*--------------------------------------------------------------------------------*/
	return t;
}

void table_insert(table *t, void *key, void *value)
{
	// fprintf(stderr, "hello nils");
	if (*t->size == MAXSIZE) // Om arrayen är full.
	{
		printf("ERROR: Can't insert. MAXSIZE reached.\n");
	}

	else
	{
		// Allokera minne för en post
		//struct table_entry *entry = malloc(sizeof(struct table_entry));
		struct table_entry *entry = malloc(sizeof(struct table_entry));
		entry->key = key;
		entry->value = value;
		/*-------------------------------------------------------------------------------------*/
		array_1d_set_value(t->entries, entry, *t->size);
		t->size++;
	}
}

bool table_is_empty(const table *t)
{
	// return array_1d_has_value(t->entries, 0); //Kollar om index 0 har ett värde, VIKTIGT se till att alltid fylla arrayen från 0 och uppåt.
	return t->size == 0;
}

void table_remove(table *t, const void *key)
{
	for (int lookup_index = 0; lookup_index < MAXSIZE; lookup_index++)
	{
		// Inspect the table entry, store it in *entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, lookup_index);
		// Check if the entry key matches the search key.
		if (t->key_cmp_func(entry->key, key) == 0)
		{
			// Markera posten som tom genom att tilldela NULL till dess värden.
			array_1d_set_value(t->entries, NULL, lookup_index);
			// Minska antalet poster i tabellen, om nödvändigt
			t->size--;
			// Frigör minnet för den borttagna posten (om det behövs)
			free(entry); // Detta kan vara valfritt beroende på hur du hanterar minnet för posterna
			break;		 // Vi har hittat och markerat posten som tom, så vi kan avbryta loopen
		}
	}
}

void table_kill(table *t)
{
	/*
		// Free all memory associated with entries
		for (int lookup_index = 0; lookup_index < MAXSIZE; lookup_index++)
		{
			struct table_entry *entry = array_1d_inspect_value(t->entries, lookup_index);
			if (entry != NULL)
			{
				if (t->key_free_func != NULL)
					t->key_free_func(entry->key);
				if (t->value_free_func != NULL)
					t->value_free_func(entry->value);
				free(entry);
			}
		}
		array_1d_kill(t->entries); // Free the array memory
		free(t);
		// Free the table memory
	*/
}

void *table_lookup(const table *t, const void *key)
{
	// Iterate over the array. Return first match.
	for (int lookup_index = 0; lookup_index < MAXSIZE; lookup_index++)
	{
		// Inspect the table entry store it in *entry
		struct table_entry *entry = array_1d_inspect_value(t->entries, lookup_index);
		// Check if the entry key matches the search key.
		if (t->key_cmp_func(entry->key, key) == 0)
		{
			// If yes, return the corresponding value pointer.
			return entry->value;
		}
	}

	// No match found. Return NULL.
	return NULL;
}

/*
int main(){
	table *t = table_empty();
	int *v = malloc(sizeof(*v));

	char *key = "walla";

	*t = table_insert(t, key, 3);
	return 0;
}
*/