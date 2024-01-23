#include <stdio.h>
#include <stdlib.h>
#include <int_stack.h>
#include <stdbool.h>
#define n 5

bool value_equal(int v1, int v2)
{
	return v1 == v2;
}

void test_stack_pop_operation(void)
{
	fprintf(stderr, "Testing stack_pop operation...\n");
	stack s = stack_empty();
	int expected_value = 4;

	for (int i = 0; i <= expected_value; i++)
	{
		s = stack_push(s, i);
	}

	while (!stack_is_empty(s))
	{
		if (value_equal(stack_top(s), expected_value))
		{
			s = stack_pop(s);
			expected_value--;
		}
		else
		{
			fprintf(stderr, "stack_pop FAIL: Expected: %d Value: %d\n",
					expected_value, stack_top(s));
			stack_kill(s);
			exit(32);
		}
	}
}

void test_stack_top_operation(void)
{
	fprintf(stderr, "Testing stack_top operation...\n");
	stack s = stack_empty();
	stack tmp = stack_push(s, 1);

	if (value_equal(stack_top(s), stack_top(tmp)))
	{
		fprintf(stderr, "FAILED: Expected 1, got %d\n", stack_top(s));
		stack_kill(s);
		exit(55);
	}
}

void test_stack_push_operation(void)
{
	fprintf(stderr, "Testing stack_push operation...\n");
	stack s = stack_empty();
	for (int expected_value = 0; expected_value < n; expected_value++)
	{
		s = stack_push(s, expected_value);
		if (!value_equal(stack_top(s), expected_value))
		{
			fprintf(stderr, "FAIL: Expected %d, got %d\n", expected_value, stack_top(s));
			stack_kill(s);
			exit(36);
		}
	}
}

void test_empty_always_returns_true(void)
{
	fprintf(stderr, "Testing if stack_isempty returns true on non-empty stack...\n");
	stack s = stack_push(stack_empty(), 1);
	if (stack_is_empty(s)) // If empty returns TRUE
	{
		stack_kill(s);
		fprintf(stderr, "FAILED: Expected 0, Recieved %d", stack_is_empty(s));
		exit(2);
	}
}

void test_empty_always_returns_false(void)
{
	fprintf(stderr, "Testing if stack_isempty returns false on empty stack...\n");
	stack s = stack_empty();
	if (!stack_is_empty(s)) // If is_empty returns FALSE
	{
		stack_kill(s);
		fprintf(stderr, "FAILED: Expected 1, Recieved %d\n", stack_is_empty(s));
		exit(1);
	}
}

void run_stack_tests(void)
{

	test_empty_always_returns_false();
	test_empty_always_returns_true();
	test_stack_push_operation();
	test_stack_top_operation();
	test_stack_pop_operation();
}

int main()
{
	run_stack_tests();

	fprintf(stderr, "SUCCESS: Implementation passed all tests. Normal exit.\n\n\n");
	return 0;
}