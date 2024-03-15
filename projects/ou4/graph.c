#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <array_2d.h>
#include <array_1d.h>
#include <dlist.h>
#include "graph.h"

struct graph
{
	array_2d *matrix;
	array_1d *nodes;
	int num_nodes;
};

struct node
{
	char *label;
	array_1d *neighbors;
	bool seen;
};


/*============================================*/
void print_node(node *n)
{
	fprintf(stderr, "%s\n", n->label);
}

static void print_slay(const void *v)
{

	const node *data = v;

	if (data == NULL)
	{
		printf(".");
	}
	else
	{
		printf("%s", data->label);
	}
}

void graph_print(const graph *g)
{
	array_1d_print(g->nodes, print_slay);
}
/*==============================================*/

static char *copy_string(const char *str)
{
	char *copy = calloc(strlen(str) + 1, sizeof(char));
	strcpy(copy, str);
	return copy;
}

bool nodes_are_equal(const node *n1, const node *n2)
{	
	return strcmp(n1->label, n2->label) == 0;
}

graph *graph_empty(int max_nodes)
{
	graph *g = calloc(1, sizeof(*g));
	g->num_nodes = 0;
	g->nodes = array_1d_create(0, max_nodes, NULL);
	g->matrix = array_2d_create(0, max_nodes, 0, max_nodes, NULL);

	return g;
}

graph *graph_insert_node(graph *g, const char *s)
{
	node *inspection_node = graph_find_node(g, s);
	if (graph_find_node(g, s) != NULL)
	{
		fprintf(stderr, "Warning: node %s already in graph\n", inspection_node->label);
		return g;
	}

	node *n = malloc(sizeof(node));
	n->label = copy_string(s);
	n->seen = false;

	array_1d_set_value(g->nodes, n, g->num_nodes);
	g->num_nodes++;
	fprintf(stderr, "Inserted node %s\n", n->label);
	return g;
}

graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
	int node_source_index = -1;
	int node_destination_index = -1;
	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_source_node = array_1d_inspect_value(g->nodes, i);
		if (nodes_are_equal(inspection_source_node, n1))
		{
			node_source_index = i;
			break;
		}
	}

	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_destination_node = array_1d_inspect_value(g->nodes, i);
		if (strcmp(inspection_destination_node->label, n2->label) == 0)
		{
			node_destination_index = i;
			break;
		}
	}

	if (node_source_index != -1 && node_destination_index != -1)
	{
		int *v = malloc(sizeof(*v));
		*v = 1;
		array_2d_set_value(g->matrix, v, node_source_index, node_destination_index);
		fprintf(stderr, "Inserted edge from %s to %s\n", n1->label, n2->label);
	}

	return g;
}

/*
void print_const_node(const node *n){
	fprintf(stderr, "%s", n->label);
}
*/

dlist *graph_neighbours(const graph *g, const node *n) //Kan vara fel för att det är en const
{
	fprintf(stderr, "inuti neighbours ");
	print_const_node(n);
	fprintf(stderr, "\n");
	
	
	dlist *neighbours = dlist_empty(NULL);
	int node_index = -1;

	for (int i = 0; i < g->num_nodes; i++)
	{
		// Jag hejar så grovt på dig
		//node *inspection_node = ;
		
		if (!array_1d_has_value(g->nodes, 0))
		{
			fprintf(stderr, "JÄVLA SKIT\n");
			exit(EXIT_FAILURE);
		}

		if (nodes_are_equal(n, array_1d_inspect_value(g->nodes, i)))
		{
			node_index = i;
		}
	}
	if (node_index == -1)
	{
		return NULL;
	}
	for (int i = 0; i < g->num_nodes; i++)
	{

		// OM NÅTT ÄR FEL ÄR DET FÖRMODLIGEN HÄR
		if (*(int *)array_2d_inspect_value(g->matrix, node_index, i) == 1)
		{
			node *inspection_node = array_1d_inspect_value(g->nodes, i);
			if (strcmp(inspection_node->label, n->label) != 0)
			{
				char label[sizeof(inspection_node)];
				strcpy(label, inspection_node->label);
				dlist_insert(neighbours, label, dlist_first(neighbours));
			}
		}
	}
	return neighbours;
}

bool graph_is_empty(const graph *g)
{
	return g->num_nodes == 0;
}

node *graph_find_node(const graph *g, const char *s)
{
	node *inspection_node;
	for (int i = 0; array_1d_has_value(g->nodes, i); i++)
	{
		inspection_node = array_1d_inspect_value(g->nodes, i);
		if (strcmp(inspection_node->label, s) == 0)
		{
			return inspection_node;
		}
	}
	return NULL;
}

bool graph_node_is_seen(const graph *g, const node *n)
{
	return n->seen;
}

graph *graph_node_set_seen(graph *g, node *n, bool seen)
{
	n->seen = seen;
	return g;
}

graph *graph_reset_seen(graph *g)
{
	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_node = array_1d_inspect_value(g->nodes, i);
		inspection_node->seen = false;
	}
	return g;
}

void graph_kill(graph *g)
{
	array_2d_kill(g->matrix);

	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_node = array_1d_inspect_value(g->nodes, i);
		free(inspection_node);
	}
	array_1d_kill(g->nodes);
	free(g);
}

//========================================================//
void array_2d_print2(const array_2d *a, inspect_callback print_func)
{

	printf("\n");
	for (int i = array_2d_low(a, 1); i <= array_2d_high(a, 1); i++)
	{

		for (int j = array_2d_low(a, 2); j <= array_2d_high(a, 2); j++)
		{
			if (array_2d_has_value(a, i, j))
			{

				print_func(array_2d_inspect_value(a, i, j));
			}
			else
			{
				printf(". ");
			}
			if (j < array_2d_high(a, 2))
			{
			}
		}
		printf("\n");
	}
	printf("\n");
}

