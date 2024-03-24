/*
 * Algorithm for finding paths between nodes in a graph created 
 * from a file and arguments inputted by the user, for the 
 * "Data Structures and Algorithms" course at the Department 
 * of Computing Science, Umeå University.
 *
 * Authors:
 *	Nils Sjölund (id23nsd@cs.umu.se)
 * 	Hanna Vigren (id23hvn@cs.umu.se)
 *  Adam Hultgren (ens22ahn@cs.umu.se)
 * 
 * Based on earlier code by: Niclas Borlin (niclas@cs.umu.se)
 * 
 *
 * Version information:
 *	v1.0 2024-04-24 first public version
 */

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

/**
 * @brief Get the edges object
 *
 * @param fp - file to reade from
 * @return int - number of edges
 */
int get_edges(FILE *fp)
{
	int edges = -1;
	char buff[BUFSIZE]; // Buff to store lines in

	//"Traverse" the file line-by-line
	while (fgets(buff, sizeof(buff), fp) != NULL)
	{

		// If the line is a comment, read the next line
		if (buff[0] == '#' || isspace(buff[0]))
		{
			continue;
		}

		// Set edges to the first non-comment line
		else
		{
			sscanf(buff, "%d", &edges);
			break;
		}
	}

	// If no edges was recieved
	if (edges == -1)
	{
		fprintf(stderr, "ERROR: file doesn't follow specifications.\n");
		exit(EXIT_FAILURE);
	}
	return edges;
}

/**
 * @brief - Find a path between a source node and
 * a destination node using a breadth-first search
 *
 * @param g - graph to inspect
 * @param src - source node
 * @param dest - destination node
 * @return true - if a path was found
 * @return false - if a path was not found
 */
bool find_path(graph *g, node *src, node *dest)
{
	// Return true if the soruce
	// and destination are equal
	if (nodes_are_equal(src, dest))
	{
		return true;
	}

	// Set the soruce node to seen
	graph_node_set_seen(g, src, true);

	// Create a queue and ad the source node
	queue *q = queue_enqueue(queue_empty(NULL), src);

	// Search for destination node until all nodes
	// have been seen or a path has been found
	while (!queue_is_empty(q))
	{
		// inspect the first node in the queue and dequeue it
		node *inspection_node = queue_front(q);
		q = queue_dequeue(q);

		// inspect its neighbours and search for the destination node
		dlist *neighbours = graph_neighbours(g, inspection_node);
		dlist_pos pos = dlist_first(neighbours);
		while (!dlist_is_end(neighbours, pos))
		{
			node *list_node = dlist_inspect(neighbours, pos);

			// If the inspected node is equal to the destination node,
			// a path has been found, kill the list and queue, reset
			// the seen-status of all nodes and return true
			if (nodes_are_equal(list_node, dest))
			{
				dlist_kill(neighbours);
				graph_reset_seen(g);
				queue_kill(q);
				return true;
			}

			// If the current neighbour to the node hasn't  already been
			// inspected, add it to the queue and set it to seen
			if (!graph_node_is_seen(g, list_node))
			{
				graph_node_set_seen(g, list_node, 1);
				q = queue_enqueue(q, list_node);
			}
			pos = dlist_next(neighbours, pos);
		}
		dlist_kill(neighbours);
	}

	// If the queue is empty: no path was found.
	// Kill the queue and return false
	queue_kill(q);
	return false;
}

/**
 * @brief - Ask for user inputs and look for a path
 * between the inputted nodes
 *
 * @param g - graph to inspect
 * @return - None
 */
void user_input_search(graph *g)
{
	// Variables to use as inputs for the find_path function
	char source_label[MAXNODENAME] = "PREVENT VALGRIND ERROR";
	char destination_label[MAXNODENAME];

	// Buff to store user input
	char search[256];

	// As long as "quit" hasn't been entered
	while (strcmp(source_label, "quit") != 0)
	{
		// Ask for user input and store the input from the
		// buff to the source- and destination-label
		printf("Enter origin and destination (quit to exit): ");
		fgets(search, sizeof(search), stdin);
		sscanf(search, "%s %s", source_label, destination_label);

		// If "quit" has been entered, kill the graph and exit the function
		if (strcmp(source_label, "quit") == 0)
		{
			graph_kill(g);
			return;
		}

		// Check if source and destination is in the graph
		node *source = graph_find_node(g, source_label);
		node *destination = graph_find_node(g, destination_label);
		if (source == NULL || destination == NULL)
		{
			fprintf(stderr, "Failure: source or destination isn't in the graph\n");
		}

		// If there exists a path between the source
		// and destination, print success
		else if (find_path(g, source, destination))
		{
			printf("There is a path from %s to %s.\n", source_label, destination_label);
		}

		// else, print failure
		else
		{
			printf("There is no path from %s to %s.\n", source_label, destination_label);
		}
	}
}

/**
 * @brief - Main function. Creates graph from inputted file
 * and runs the interactive search function on the graph.
 *
 *
 * @param argc - number of command-line arguments
 * @param argv - String of command-line arguments
 * @return int - 0 on successfull run. Non-zero on failure.
 */
int main(int argc, char *argv[])
{
	// Open file from command-line argument and
	// store it in a file-pointer
	const char *filename = argv[1];
	FILE *fp;
	fp = fopen(filename, "r");

	// If the file didn't open, print
	// error-message and exit the program
	if (fp == NULL)
	{
		fprintf(stderr, "ERROR: file didn't open\n");
		exit(EXIT_FAILURE);
	}

	// Store the number of edges and create a
	// graph with twice as many maxnodes (worst case)
	int edges = get_edges(fp);
	graph *g = graph_empty(2 * edges);

	// Buff for storing lines from the file
	char buff[BUFSIZE];

	// Read the file line by line until its end is reached
	while (fgets(buff, BUFSIZE, fp) != NULL)
	{
		// If the line isn't a comment or a digit
		if (!line_is_comment(buff) && !isdigit(buff[0]))
		{
			// Create tokens from the line and store the first token
			// as a source-label, and second as a destination-label
			char *source_node_label = strtok(buff, " \n\t");
			char *destination_node_label = strtok(NULL, " \n\t");

			// Create nodes with the right labels in the graph
			g = graph_insert_node(g, source_node_label);
			g = graph_insert_node(g, destination_node_label);

			// Get the source and destination nodes from the graph
			// and insert an edge between them
			node *source_node = graph_find_node(g, source_node_label);
			node *destination_node = graph_find_node(g, destination_node_label);
			g = graph_insert_edge(g, source_node, destination_node);
		}
	}

	// Run the interactive search function on the created graph
	user_input_search(g);

	// When done, close the file
	fclose(fp);
	printf("Normal exit.\n");
	return 0;
}