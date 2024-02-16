#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h" // Använd din egen headerfil

#define VERSION "v1.1"
#define VERSION_DATE "2023-01-14"

// Skapa en dynamisk kopia av strängen str.
static char* make_string_copy(const char *str)
{
    char *copy = calloc(strlen(str) + 1, sizeof(char));
    strcpy(copy, str);
    return copy;
}

// Tolka de tillhandahållna nyckel- och värdepekarna och skriv ut deras innehåll.
static void print_int_string_pair(const void *key, const void *value)
{
    const int *k = key;
    const char *s = value;
    printf("[%d, %s]\n", *k, s);
}

// Jämför två nycklar (int *).
static int compare_ints(const void *k1, const void *k2)
{
    int key1 = *(int *)k1;
    int key2 = *(int *)k2;

    if (key1 == key2)
        return 0;
    if (key1 < key2)
        return -1;
    return 1;
}

int main(void)
{
    printf("%s, %s %s: Skapa en (integer, string) tabell utan minnesfrihanterare.\n",
           __FILE__, VERSION, VERSION_DATE);
    printf("Versionsnummer %s.\n\n", VERSION);

    // Håll reda på nyckel-värde-par som vi allokerar.
    int *keys[10];
    char *values[10];

    table *t = table_empty(compare_ints, NULL, NULL);

    // Skapa och sätt in 10 olika poster med olika nycklar.
    for (int i = 0; i < 10; i++) {
        keys[i] = malloc(sizeof(int));
        *keys[i] = 10 - i; // Nycklar från 1 till 10
        char buffer[20];
        sprintf(buffer, "Value %d", 10 - i);
        values[i] = make_string_copy(buffer);
        table_insert(t, keys[i], values[i]);
    }

    // Skriv ut tabellen efter att ha satt in alla poster.
    printf("Tabell efter att ha satt in 10 poster:\n");
    table_print(t, print_int_string_pair);

    // Fri nycklar och värden
    for (int i = 0; i < 10; i++) {
        free(keys[i]);
        free(values[i]);
    }

    // Avsluta programmet
    printf("\nNormal avslut.\n\n");
    return 0;
}
