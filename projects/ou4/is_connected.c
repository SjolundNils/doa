#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "graph.h"
#include <queue.h>
#include <string.h>
#include <array_1d.h>

#define MAXNODENAME 40
#define BUFSIZE 400

/**
 * @brief Find position of first non-whitespace character.
 *
 * @param s - string to search.
 * @return int - Returns the position of the first non-whitespace character, or -1 if not found.
 */
int first_non_white_space(const char *s)
{
	int i = 0; // Start at first char.

	// Advance as long as we're loooking at white-space until we hit EOL.
	while (s[i] && isspace(s[i]))
	{
		i++;
	}
	// We found a non-whitespace char.
	if (s[i])
	{
		// It was a proper characted. Return its position.
		return i;
	}
	else
	{
		// It was the null terminator. Return fail.
		return -1;
	}
}

static void print_strings(void *data)
{
	char *s = data;
	printf("%s", s);
}

/**
 * @brief Determine if the string is blank.
 *
 * @param s - string to search.
 * @return true if the line contains only whitespace chars.
 * @return false if the line contains at least one non-whitespace char.
 */

bool line_is_blank(const char *s)
{
	// Line is blank if it only contained white-space chars.
	return first_non_white_space(s) < 0;
}

/**
 * @brief Determine if the string is a comment line.
 *
 * @param s - string to search.
 * @return true if the line is a comment line.
 * @return false if the line is not a comment line.
 *
 * A comment line has a hash sign '#' as the first non-whitespace char on the line.
 */
bool line_is_comment(const char *s)
{
	int i = first_non_white_space(s);
	return (i >= 0 && s[i] == '#');
}

/**
 * @brief Extract node names from a line from the map file.
 *
 * @param buf - Input buffer.
 * @param n1 - Output buffer for the first node name. Must be at least MAXNODENAME+1 long.
 * @param n2 - Ditto for the second node name.
 * @return int - Returns the number of correctly parsed node names. If the return value is 2, both n1
 * and n2 contain node names. If the return value is less than 2, parsing of at least one node name
 * failed, in which case the content of n1 and n2 are undefined.
 */
int parse_map_line(const char *buf, char *n1, char *n2)
{
	// Create a format string the will do the work.
	char fmt[20];
	// This will generate the format string " %40s %40s" if MAXNODENAME is 40.
	snprintf(fmt, sizeof(fmt), " %%%ds %%%ds", MAXNODENAME, MAXNODENAME);

	// sscanf does all the necessary parsing.
	// Node names must be separated by whitespace.
	// Whitespace before the first node name is allowed.
	// Anything after the second node name is ignored.
	int n = sscanf(buf, fmt, n1, n2);

	// The return value from sscanf contains the number of properly parsed format codes, i.e. the
	// number of node names.
	return n;
}

int get_edges(FILE *fp)
{
	int edges = -1;
	char buff[BUFSIZE];
	while (fgets(buff, sizeof(buff), fp) != NULL)
	{
		if (buff[0] == '#' || isspace(buff[0]))
		{
			continue;
		}
		else
		{
			sscanf(buff, "%d", &edges);
			break;
		}
	}

	if (edges == -1)
	{
		exit(EXIT_FAILURE);
	}
	return edges;
}

bool find_path(graph *g, node *src, node *dest)
{

	if (nodes_are_equal(src, dest))
	{
		return true;
	}
	graph_node_set_seen(g, src, true);

	queue *q = queue_enqueue(queue_empty(NULL), src);

	while (!queue_is_empty(q))
	{


		node *inspection_node = queue_front(q);
		q = queue_dequeue(q);

		dlist *neighbours = graph_neighbours(g, inspection_node);

		dlist_print(neighbours, print_strings);

		dlist_pos pos = dlist_first(neighbours);

		while (!dlist_is_end(neighbours, pos))
		{
			node *list_node = dlist_inspect(neighbours, pos);

			if (list_node == NULL)
			{

				exit(EXIT_FAILURE);
			}

			if (nodes_are_equal(list_node, dest))
			{
				dlist_kill(neighbours);
				graph_reset_seen(g);
				queue_kill(q);
				return true;
			}
			if (!graph_node_is_seen(g, list_node))
			{
				graph_node_set_seen(g, list_node, 1);
				q = queue_enqueue(q, list_node);
			}
			pos = dlist_next(neighbours, pos);
		}
		dlist_kill(neighbours);
	}
	queue_kill(q);
	return false;
}

void run_input(graph *g)
{
	char source_label[MAXNODENAME];
	char destination_label[MAXNODENAME];
	char search[256];
	while (strcmp(source_label, "quit") != 0)
	{
		fprintf(stderr, "Enter origin and destination (quit to exit): ");

		fgets(search, sizeof(search), stdin);
		sscanf(search, "%s %s", source_label, destination_label);

		// Det är här problemen uppstår
		node *source = graph_find_node(g, source_label);
		// fprintf(stderr, "%s\n", source->label);

		node *destination = graph_find_node(g, destination_label);

		if (source == NULL || destination == NULL)
		{
			fprintf(stderr, "Failure: source or destination isn't in the graph\n");
			exit(EXIT_FAILURE);
		}

		bool path = find_path(g, source, destination);

		if (path == 1)
		{
			printf("There is a path from %s to %s :)\n", source_label, destination_label);
		}
		else
		{
			printf("There is not :(\n");
		}
	}
}

int main(int argc, char *argv[])
{
	const char *filename = argv[1];
	FILE *fp;
	fp = fopen(filename, "r");

	if (fp == NULL)
	{
		fprintf(stderr, "ERROR: file didn't open\n");
		exit(EXIT_FAILURE);
	}

	int edges = get_edges(fp);
	graph *g = graph_empty(2 * edges);
	char buff[BUFSIZE];

	while (fgets(buff, BUFSIZE, fp) != NULL)
	{
		if (!line_is_comment(buff) && !isdigit(buff[0]))
		{
			char *source_node_label = strtok(buff, " \n\t");
			char *destination_node_label = strtok(NULL, " \n\t");

			g = graph_insert_node(g, source_node_label);
			g = graph_insert_node(g, destination_node_label);

			node *source_node = graph_find_node(g, source_node_label);

			node *destination_node = graph_find_node(g, destination_node_label);

			g = graph_insert_edge(g, source_node, destination_node);
			print_node(source_node);
		}
	}

	run_input(g);

	return 0;
}