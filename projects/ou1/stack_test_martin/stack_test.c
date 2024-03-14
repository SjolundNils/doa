#include <stdio.h>
#include <stdlib.h>
#include <stack.h>
#include <stdbool.h>

/*
 * Test program for the stack implementation in stack.c
 *
 * Author: Martin Engstrom (id23mem@cs.umu.se)
 *
 * Version information:
 * 2023-02-14 v1.0. First public version
 */

/*
 * Function comparing values from stacks
 */
bool compare_values(int v1, int v2)
{
	return v1 == v2;
}

/*
 * check_stack_empty() - using a empty stack to test return
 *
 * Preconditions: stack s is empty
 */
void check_stack_empty()
{
	fprintf(stderr, "\n\nTesting empty stack\n\n");

	stack *s = stack_empty(free);

	if (stack_is_empty(s) == 0)
	{
		fprintf(stderr, "FAIL: Expected empty stack, got return false.\n");
		exit(0);
	}
	stack_kill(s);
}

/*
 * check_stack_not_empty() - making a not empty stack to test return
 *
 * Preconditions: stack s is empty
 */
void check_stack_not_empty()
{
	fprintf(stderr, "\n\nTesting what empty stack returns\n\n");

	stack *s = stack_empty(free);
	int *v = malloc(sizeof(*v));
	*v = 3;
	s = stack_push(s, v);

	if (stack_is_empty(s) == 1)
	{
		fprintf(stderr, "FAIL: Expected not-empty stack, got return false.\n");
		exit(0);
	}
	stack_kill(s);
}

/*
 * check_push_top_value() - testing stack after push with its top value
 *
 * Preconditions: stack s is empty
 */
void check_push_top_value()
{
	fprintf(stderr, "\n\nTesting top value and push with top\n\n");

	stack *s = stack_empty(free);
	int *v = malloc(sizeof(*v));
	*v = 3;
	s = stack_push(s, v);

	if (compare_values(*(int *)stack_top(s), *v) == 0)
	{
		fprintf(stderr, "Fail: Ecpected top value, but got other.\n");
		exit(0);
	}
	stack_kill(s);
}

/*
 * check_push_whole_stack() - testing all values in stack after push
 *
 * Preconditions: stack s is empty
 */
void check_push_whole_stack()
{
	fprintf(stderr, "\n\nTesting stack efter push\n\n");

	stack *s = stack_empty(free);

	for (int i = 0; i < 3; i++)
	{
		int *m = malloc(sizeof(*m));
		*m = i;
		s = stack_push(s, m);

		if (*(int *)stack_top(s) != i)
		{
			fprintf(stderr, "Fail: Ecpected top value, but got other.\n");
			exit(0);
		}
	}
	stack_kill(s);
}

/*
 * check_stack_top() - testing top value of stack
 *
 * Preconditions: stack s is empty
 */
void check_stack_top()
{
	fprintf(stderr, "\n\nTesting stack top\n\n");

	stack *s = stack_empty(free);

	for (int i = 0; i < 3; i++)
	{
		int *m = malloc(sizeof(m));
		*m = i;
		s = stack_push(s, m);

		if (compare_values(*(int *)stack_top(s), i) == 0)
		{
			fprintf(stderr, "Fail: Excpected top value, but got other.\n");
			exit(0);
		}
	}
	stack_kill(s);
}

/*
 * check_stack_pop() - testing stack after pop with top value
 *
 * Preconditions: stack s is empty
 */
void check_stack_pop()
{
	fprintf(stderr, "\n\nTesting stack pop\n\n");

	int n = 4;
	stack *s = stack_empty(free);

	for (int i = 0; i < n; i++)
	{
		int *m = malloc(sizeof(m));
		*m = i;
		s = stack_push(s, m);
	}

	int expected_value = n - 1;

	for (int i = 0; i < n; i++)
	{
		if (compare_values(*(int *)stack_top(s), expected_value) == 0)
		{
			fprintf(stderr, "Fail: Expected value %d, but got %d.\n", expected_value, *(int *)stack_top(s));
			exit(0);
		}
		s = stack_pop(s);
		expected_value--;
	}
	stack_kill(s);
}

/*
 * Function that runs all other functions
 *
 * Preconditions: all functions work
 */
void run_all_checks()
{
	check_stack_empty();
	check_stack_not_empty();
	check_push_top_value();
	check_push_whole_stack();
	check_stack_top();
	check_stack_pop();

	fprintf(stderr, "SUCCESS: Implementation passed all tests. Normal exit.\n");
}

/*
 * Main function that executes full program with all its functions
 */
int main()
{
	run_all_checks();

	return 0;
}