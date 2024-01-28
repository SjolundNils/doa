#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stack.h>

#define n 5


bool value_equal(int v1, int v2)
{
	return v1 == v2;
}

void test_push(void)
{
	fprintf(stderr, "Testing stack_push...\n");

	stack *s = stack_empty(NULL);

	for (int expected_value = 0; expected_value < n; expected_value++)
	{
		int *v = malloc(sizeof(*v));
		*v = expected_value;
		s = stack_push(s, v);

		int *s_top = stack_top(s);
		if (*s_top != expected_value)
		{
			fprintf(stderr, "FAIL: expected %d, got %d", *s_top, expected_value);
			exit(EXIT_FAILURE);
		}
	}

	while (!stack_is_empty(s))
	{
		int *v = stack_top(s);
		s = stack_pop(s);
		free(v);
	}
	stack_kill(s);
}

void test_is_empty_one_element(void)
{
	fprintf(stderr, "Testing if empty always returns true...\n");
	int *v = malloc(sizeof(*v));
	*v = 1;

	stack *s = stack_push(stack_empty(NULL), v);

	if (stack_is_empty(s)) // If is_empty returns TRUE
	{
		fprintf(stderr, "FAILED: Expected FALSE, got %s\n", stack_is_empty(s) ? "TRUE" : "FALSE");
		exit(EXIT_FAILURE);
	}

	while (!stack_is_empty(s))
	{
		int *v = stack_top(s);
		s = stack_pop(s);
		free(v);
	}
	stack_kill(s);
}

void test_empty_returns_false(void)
{
	fprintf(stderr, "Testing if stack_is_empty returns false on empty stack...\n");
	stack *s = stack_empty(NULL);
	if (!stack_is_empty(s)) // If is_empty returns FALSE
	{
		fprintf(stderr, "FAILED: Expected TRUE, got %s\n", stack_is_empty(s) ? "TRUE" : "FALSE");
		exit(1);
	}
	stack_kill(s);
}

void test_empty_one_elemet()
{
	fprintf(stderr, "Testing stack_empty...\n");

	stack *s = stack_empty(NULL);
	if (s == NULL)
	{
		fprintf(stderr, "FAIL: expected non-NULL, got NULL\n");
		exit(EXIT_FAILURE);
	}

	while (!stack_is_empty(s))
	{
		int *v = stack_top(s);
		s = stack_pop(s);
		free(v);
	}
	stack_kill(s);
}

void run_stack_tests(void)
{
	test_empty_returns_false();
	test_empty_one_elemet();
	test_push();
	// test_empty_always_returns_true();
	// test_stack_push_operation();
	// test_stack_top_operation();
	// test_stack_pop_operation();
}

int main()
{
	run_stack_tests();

	fprintf(stderr, "SUCCESS: Implementation passed all tests. Normal exit.\n\n\n");
	return 0;
}