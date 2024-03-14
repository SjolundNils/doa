#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <array_2d.h>
#include <array_1d.h>
#include "graph.h"

struct graph
{
	array_2d *matrix;
	array_1d *nodes;
	int num_nodes;
};

struct node
{
	const char *label;
	array_1d *neighbors;
	bool seen;
};

bool nodes_are_equal(const node *n1, const node *n2)
{
	return (strcmp(n1->label, n2->label) == 0);
}

graph *graph_empty(int max_nodes)
{
	graph *g = malloc(sizeof(*g));
	g->num_nodes = 0;

	g->matrix = array_2d_create(0, max_nodes, 0, max_nodes, NULL);

	return g;
}

graph *insert_node(graph *g, const char *s)
{
	node *n = malloc(sizeof(n));
	n->label = s;
	n->seen = false;

	array_1d_set_value(g->nodes, n, g->num_nodes);
	return g;
}

graph *graph_insert_edge(graph *g, node *n1, node *n2)
{
	int node_source_index = -1;
	int node_destination_index = -1;
	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_source_node = array_1d_inspect_value(g->nodes, i);
		if (strcmp(inspection_source_node->label, n1) == 0)
		{
			node_source_index = i;
		}
	}

	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_destination_node = array_1d_inspect_value(g->nodes, i);
		if (strcmp(inspection_destination_node->label, n2) == 0)
		{
			node_destination_index = i;
		}
	}

	if (node_source_index != -1 && node_destination_index != -1)
	{
		array_2d_set_value(g->matrix, 1, node_source_index, node_destination_index);
	}
}

dlist *graph_neighbours(const graph *g, const node *n)
{
	neighbours = dlist_empty(free);
	int node_index = -1;

	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_node = array_1d_inspect_value(g->nodes, i);
		if (strcmp(inspection_node->label, n) == 0)
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
		if (array_2d_inspect_value(g->matrix, i, node_index) == 1)
		{
			node *inspection_node = array_1d_inspect_value(g->nodes, i);
			if (strcmp(inspection_node->label, n->label) != 0)
			{
				dlist_insert(neighbours, inspection_node->label, dlist_first(neighbours));
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

	for (int i = 0; i < g->num_nodes; i++)
	{
		node *inspection_node = array_1d_inspect_value(g->nodes, i);
		if (strcmp(inspection_node->label, s) == 0)
		{
			return inspection_node;
		}
	}
	return NULL;
}

bool node_is_seen(const graph *g, node *n)
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

int main()
{
	printf("DEn funkar\n");
	return 0;
}