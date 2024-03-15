/*==================HELPER FUNCTIONS
void print_node(node *n)
{
	fprintf(stderr, "%s\n", n->label);
}

static void print_matrix(const void *v)
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
	array_1d_print(g->nodes, print_matrix);
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

=======================*/