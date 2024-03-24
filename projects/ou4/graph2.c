/*
 * dlist-based graph implementation tailored for use 
 * by "is_connected.c" in the "Data Structures and Algorithms" course 
 * at the Department of Computing Science, Umeå University.
 *
 * Authors:
 *	Nils Sjölund (id23nsd@cs.umu.se)
 * 	Hanna Vigren (id23hvn@cs.umu.se)
 *  Adam Hultgren (ens22ahn@cs.umu.se) 
 *
 * Version information:
 *	v1.0 2024-04-24 first public version
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <array_2d.h>
#include <array_1d.h>
#include <dlist.h>
#include "graph.h"

// ===========INTERNAL DATA TYPES============

/*
 * Graph representation consisting an array of nodes.
 *
 * "num_nodes" tracks the total number of nodes in the graph
 * (used as an index when inserting new nodes, and in graph_is_empty).
 *
 */
struct graph
{
	array_1d *nodes;
	int num_nodes;
};

/*
 * Node representation containing a label and a boolean flag indicating whether
 * the node has been visited or not during graph traversal, 
 * as well as a list of its neighbours
 */
struct node
{
	dlist *neighbours;
	char *label;
	bool seen;
};

// ===========INTERNAL HELP FUNCTIONS============

/**
 * @brief - Copies the input string "str" into a newly
 * allocated memory block and returns a pointer to it.
 * This function is used internally for creating copies of strings.
 *
 * @param str - string to copy
 * @return char* - copy of given string
 */
static char *copy_string(const char *str)
{
	char *copy = calloc(strlen(str) + 1, sizeof(char));
	strcpy(copy, str);
	return copy;
}

/**
 * @brief - Checks whether two nodes have equal labels.
 *
 * @param n1 - node to compare with
 * @param n2 - node to compare with
 * @return true - if the labels are identical
 * @return false - otherwise
 */
bool nodes_are_equal(const node *n1, const node *n2)
{
	return strcmp(n1->label, n2->label) == 0;
}


// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * @brief - Create a new empty graph
 * 
 * @param max_nodes - max number of nodes allowed
 * @return graph* - pointer to the created graph
 */
graph *graph_empty(int max_nodes)
{
	// Allocade memory for a new struct. Set the created graphs number of
	// nodes to zero and create its node-array
	// using "max_nodes" as high-limit.
	graph *g = calloc(1, sizeof(*g));
	g->nodes = array_1d_create(0, max_nodes, NULL);
	g->num_nodes = 0;

	// Return pointer to the created graph
	return g;
}

/**
 * @brief - Insert a node with the given label "s" into the graph "g"
 *
 * @param g - Graph to insert into
 * @param s - label of node to insert
 * @return graph* - pointer to the new, modified graph
 */
graph *graph_insert_node(graph *g, const char *s)
{
	// If the node already exists in the graph, return.
	if (graph_find_node(g, s) != NULL)
	{
		return g;
	}

	// Allocate memory for a new node, set its label to
	// the inputted string, and its seen status to false
	node *n = malloc(sizeof(*n));
	n->label = copy_string(s);
	n->seen = false;

	// Create a list for storing the nieghbours of the created node
	n->neighbours = dlist_empty(NULL);

	// Insert the node into the array at its first free index,
	// increase the graphs number of nodes, and return the modified graph
	array_1d_set_value(g->nodes, n, g->num_nodes);
	g->num_nodes++;
	return g;
}

/**
 * @brief - Inserts an edge between two given nodes in the graph "g"
 *
 * @param g - Graph to insert edge into
 * @param n1 - source-node
 * @param n2 - destination-node
 * @return graph* - pointer to the new, modified graph
 */
graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
	//Insert n2 into the list of neighbours of n1
	dlist_insert(n1->neighbours, n2, dlist_first(n1->neighbours));
	
	//Return pointer to the modified graph
	return g;
}


/**
 * @brief - Returns a list of neighbours to a given node.
 *
 * The list has to be dlist_kill:ed after use.
 *
 * @param g - Graph to inspect
 * @param n - node to inspect
 * @return dlist* - Directed list with all neighbours to "n"
 */
dlist *graph_neighbours(const graph *g, const node *n)
{
	// Create a new list to fill with neighbours
	dlist *neighbours = dlist_empty(NULL);

	// Travese the list of neighbours and copy the nodes
	// one by one into to the new list 
	dlist_pos pos = dlist_first(n->neighbours);
	while (!dlist_is_end(n->neighbours, pos))
	{
		node *inspection_node = dlist_inspect(n->neighbours, pos);
		dlist_insert(neighbours, inspection_node, dlist_first(neighbours));
		pos = dlist_next(n->neighbours, pos);
	}

	// Return the new list
	return neighbours;
}

/**
 * @brief - Checks if a graph is empty
 *
 * @param g - Graph to inspect
 * @return true - If the graph is empty
 * @return false - If the graph isn't empty
 */
bool graph_is_empty(const graph *g)
{
	return g->num_nodes == 0;
}


/**
 * @brief - Takes a string and returns a pointer to a node
 * 			with a label equal to the given string
 *
 * @param g - Graph to inspect
 * @param s - String to find corresponding node to
 * @return node* - node with label equal to given string
 */
node *graph_find_node(const graph *g, const char *s)
{
	// Traverse g->nodes and look for node with corresponding label
	node *inspection_node;
	for (int i = 0; array_1d_has_value(g->nodes, i); i++)
	{
		inspection_node = array_1d_inspect_value(g->nodes, i);
		
		// If the label of the inspected node matches, return a pointer to it
		if (strcmp(inspection_node->label, s) == 0)
		{
			return inspection_node;
		}
	}
	
	// If no match was found, return NULL
	return NULL;
}

/**
 * @brief - Check the seen-status of given node in graph "g"
 *
 * @param g - Graph to inspect
 * @param n - Node to inspect
 * @return true - if the node is seen
 * @return false - if the node is not seen
 */
bool graph_node_is_seen(const graph *g, const node *n)
{
	return n->seen;
}


/**
 * @brief - Set the seen status of given node in graph "g"
 *
 * @param g - Graph to modify
 * @param n - node to modify
 * @param seen - status to set node
 * @return graph* - the modified graph
 */
graph *graph_node_set_seen(graph *g, node *n, bool seen)
{
	n->seen = seen;
	return g;
}

/**
 * @brief - Reset the seen status of all nodes a in given graph
 *
 * @param g - Graph to modify
 * @return graph* - the modified graph
 */
graph *graph_reset_seen(graph *g)
{
	// Iterate over all nodes and set its seen-status to false
	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_node = array_1d_inspect_value(g->nodes, i);
		inspection_node->seen = false;
	}

	// Return the modified graph
	return g;
}

/**
 * @brief - Destroy a given graph
 *
 * @param g - Graph to destroyg
 */
void graph_kill(graph *g)
{
	// Iterate over all the nodes, Free its labels, 
	// kill its neighbours-lists, and the nodes themselves
	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_node = array_1d_inspect_value(g->nodes, i);
		free(inspection_node->label);
		dlist_kill(inspection_node->neighbours);
		free(inspection_node);
	}

	// Kill whats left of the array
	array_1d_kill(g->nodes);

	// And free the graph struct
	free(g);
}