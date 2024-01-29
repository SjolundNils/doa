#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <int_stack.h>
#define n 5


bool value_equal(int v1, int v2)
{
	return v1 == v2;
}


void decreasing_loop_test(void)
{
	fprintf(stderr, "Testing stack with decreasing values...\n");
	
	stack s = stack_empty();
	
	int expected_value = n - 1;

	for (int i = 0; i < n; i++)
	{
		s = stack_push(s, i);
	}

	while (!stack_is_empty(s))
	{
		if (!value_equal(stack_top(s), expected_value))
		{
			fprintf(stderr, "FAIL: Expected %d, got %d\n", expected_value, stack_top(s));
			exit(EXIT_FAILURE);
		}

		s = stack_pop(s);
		expected_value--;
	}
}


void increasing_loop_test(void)
{
	fprintf(stderr, "Testing stack with increasing values...\n");
	
	stack s = stack_empty();
	
	for (int expected_value = 0; expected_value < n; expected_value++)
	{
		s = stack_push(s, expected_value);
		
		if (!value_equal(stack_top(s), expected_value))
		{
			fprintf(stderr, "FAIL: Expected %d, got %d\n", expected_value, stack_top(s));
			exit(EXIT_FAILURE);
		}
	}
}

void is_empty_true_test(void)
{
	fprintf(stderr, "Testing if is_empty returns true on non-empty stack...\n");
	stack s = stack_push(stack_empty(), 1);
	
	if (stack_is_empty(s)) // If empty returns TRUE
	{
		fprintf(stderr, "FAILED: Expected 0, Recieved %d", stack_is_empty(s));
		exit(EXIT_FAILURE);
	}
}

void is_empty_false_test(void)
{
	fprintf(stderr, "Testing if is_empty returns false on empty stack...\n");
	stack s = stack_empty();

	if (!stack_is_empty(s)) // If is_empty returns FALSE
	{
		fprintf(stderr, "FAILED: Expected 1, Recieved %d\n", stack_is_empty(s));
		exit(EXIT_FAILURE);
	}
}

void run_stack_tests(void)
{
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