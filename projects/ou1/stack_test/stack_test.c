#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stack.h>

#define n 5


bool value_equal(int v1, int v2)
{
	return v1 == v2;
}

void decreasing_loop_test(void)
{
	fprintf(stderr, "Testing stack with decreasing values...\n");
	
	stack *s = stack_empty(NULL);
	
	int expected_value = n - 1;
	
	for (int i = 0; i < expected_value + 1; i++)
	{
		int *v = malloc(sizeof(*v));
		*v = i;
		s = stack_push(s, v);
	}
	
	while (!stack_is_empty(s))
	{
		int *s_top = stack_top(s);
		if (!value_equal(*s_top, expected_value))
		{
			fprintf(stderr, "FAIL: Expected %d, got %d\n", expected_value, *s_top);
		}
		
		free(s_top);
		s = stack_pop(s);
		expected_value--;
	}
	
	//Free memory and kill stack
	while (!stack_is_empty(s))
	{
		int *v = stack_top(s);
		s = stack_pop(s);
		free(v);
	}
	stack_kill(s);
}

void increasing_loop_test(void)
{
	fprintf(stderr, "Testing stack with increasing values...\n");

	stack *s = stack_empty(NULL);

	for (int expected_value = 0; expected_value < n; expected_value++)
	{
		int *v = malloc(sizeof(*v));
		*v = expected_value;
		s = stack_push(s, v);

		int *s_top = stack_top(s);
		if (!value_equal(*s_top, expected_value))
		{
			fprintf(stderr, "FAIL: expected %d, got %d", expected_value, *s_top);
			exit(EXIT_FAILURE);
		}
	}

	//Free memory and kill stack
	while (!stack_is_empty(s))
	{
		int *v = stack_top(s);
		free(v);
		s = stack_pop(s);
	}
	stack_kill(s);
}

void is_empty_true_test(void)
{
	fprintf(stderr, "Testing if is_empty returns true on non-empty stack...\n");
	int *v = malloc(sizeof(*v));
	*v = 1;

	stack *s = stack_push(stack_empty(NULL), v);

	if (stack_is_empty(s)) // If is_empty returns TRUE
	{
		fprintf(stderr, "FAILED: Expected FALSE, got %s\n", stack_is_empty(s) ? "TRUE" : "FALSE");
		exit(EXIT_FAILURE);
	}

	//Free memory and kill stack
	while (!stack_is_empty(s))
	{
		int *v = stack_top(s);
		s = stack_pop(s);
		free(v);
	}
	stack_kill(s);
}

void is_empty_false_test(void)
{
	fprintf(stderr, "Testing if is_empty returns false on empty stack...\n");
	stack *s = stack_empty(NULL);
	
	if (!stack_is_empty(s)) // If is_empty returns FALSE
	{
		fprintf(stderr, "FAILED: Expected TRUE, got %s\n", stack_is_empty(s) ? "TRUE" : "FALSE");
		exit(EXIT_FAILURE);
	}

	//Free memory and kill stack
	stack_kill(s);
}

void empty_test()
{
	fprintf(stderr, "Testing stack_empty...\n");

	stack *s = stack_empty(NULL);
	if (s == NULL)
	{
		fprintf(stderr, "FAIL: expected non-NULL, got NULL\n");
		exit(EXIT_FAILURE);
	}

	//Free memory and kill stack
	stack_kill(s);
}

void run_stack_tests(void)
{
	empty_test();
	is_empty_false_test();
	is_empty_true_test();
	increasing_loop_test();
	decreasing_loop_test();
}

int main()
{
	run_stack_tests();

	fprintf(stderr, "SUCCESS: Implementation passed all tests. Normal exit.\n\n\n");
	return 0;
}