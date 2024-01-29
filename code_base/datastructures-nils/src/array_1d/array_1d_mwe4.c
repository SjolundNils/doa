#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <array_1d.h>

/*
 * Minimum working example 4 for array_1d.c.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2018-04-03: Split into simpler, shorter versions.
 *   v1.2  2023-01-14: Added printouts at start/end of main.
 */

#define VERSION "v1.2"
#define VERSION_DATE "2023-01-14"

// Struct with month name and number of days.
struct month {
	char *name;
	int days;
};

// Months are stored via void pointers. Convert the given pointer and
// print the dereferenced values.
static void print_months(const void *data)
{
	// Convert void pointer to pointer to month.
	const struct month *m=data;
	printf("(%s, %d)", m->name, m->days);
}

// Function to free both month structure and char * with name.
static void free_month_struct(void *data)
{
	// Convert void pointer to pointer to month.
	struct month *m=data;
	free(m->name);
	free(m);
}

int main(void)
{
	printf("%s, %s %s: Create month list with custom memfreehandler.\n",
	       __FILE__, VERSION, VERSION_DATE);
	printf("Code base version %s.\n\n", CODE_BASE_VERSION);

	const char * months[12] = {
		"Jan", "Feb", "Mar", "Apr", "May", "Jun",
		"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
	};
	int month_days[12] = {
		31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31
	};

	// Create an array with 12 positions.
	array_1d *a = array_1d_create(1,12,free_month_struct);

	for (int i=array_1d_low(a); i<=array_1d_high(a); i++) {
		// Allocate memory for a month structure.
		struct month *m=malloc(sizeof(*m));
		// Allocate memory for month name.
		m->name=calloc(strlen(months[i-1])+1,sizeof(char));
		// Copy string.
		strcpy(m->name, months[i-1]);
		// Set days.
		m->days=month_days[i-1];
		// Set value in array.
		array_1d_set_value(a,m,i);
	}
	printf("Array after inserting 12 month structures (Jan, 31), ..., (Dec, 31):\n");
	array_1d_print(a, print_months);

	array_1d_kill(a);

	printf("\nNormal exit.\n\n");
	return 0;
}
