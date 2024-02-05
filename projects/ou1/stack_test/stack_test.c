#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stack.h>
#define SIZE 5
/*
 * Test program for the stack implementation in stack.c
 * 
 * Author: Nils Sjölund (id23nsd@cs.umu.se)
 * 
 * Version information:
 * 	2023-02-01 v1.0. First public version
 * 	2023-02-05 v1.1. Fixed comments
 */

/*
 * Function to compare the values stored in the stack. 
 */
bool value_equal(int v1, int v2)
{
	return v1 == v2;
}

/*
 * decreasing_loop_test() - Test a filled stack by comparing an 
 * expected value with the top value, using stack_top(), after using stack_pop() on the stack
 * 
 * Preconditions: stack_empty(), stack_is_empty() works.
 */
void decreasing_loop_test(void)
{
	fprintf(stderr, "Testing stack with decreasing values...\n");
	
	stack *s = stack_empty(free);
	
	int expected_value = SIZE - 1;
	
	for (int i = 0; i < SIZE; i++)
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
			fprintf(stderr, "FAIL: Expected %d, got %d\n\n", expected_value, *s_top);
			exit(EXIT_FAILURE);
		}
		
		s = stack_pop(s);
		expected_value--;
	}
	stack_kill(s);
}


/*
 * increasing_loop_test() - Test a stack by filling it, using stack_push() 
 * and simultaneously comparing the expected value with the top value, 
 * using stack_top.
 * 
 * Preconditions: stack_empty() works.
 */
void increasing_loop_test(void)
{
	fprintf(stderr, "Testing stack with increasing values...\n");

	stack *s = stack_empty(free);

	for (int expected_value = 0; expected_value < SIZE; expected_value++)
	{
		int *v = malloc(sizeof(*v));
		*v = expected_value;
		s = stack_push(s, v);

		int *s_top = stack_top(s);
		if (!value_equal(*s_top, expected_value))
		{
			fprintf(stderr, "FAIL: expected %d, got %d\n\n", expected_value, *s_top);
			exit(EXIT_FAILURE);
		}
	}

	stack_kill(s);
}

/*
 * is_empty_true_test() - Test if stack_is_empty returns TRUE
 * on a non-empty stack.
 * 
 * Preconditions: stack_empty() works.
 */
void is_empty_true_test(void)
{
	fprintf(stderr, "Testing if is_empty returns true on non-empty stack...\n");
	int *v = malloc(sizeof(*v));
	*v = 1;

	stack *s = stack_push(stack_empty(free), v);

	if (stack_is_empty(s)) // If is_empty returns TRUE
	{
		fprintf(stderr, "FAIL: Expected FALSE, got TRUE\n\n");
		exit(EXIT_FAILURE);
	}

	stack_kill(s);
}

/*
 * is_empty_false_test() - Test if stack_is_empty returns FALSE
 * on an empty stack.
 * 
 * Preconditions: stack_empty() works.
 */
void is_empty_false_test(void)
{
	fprintf(stderr, "Testing if is_empty returns false on empty stack...\n");
	stack *s = stack_empty(free);
	
	if (!stack_is_empty(s)) // If is_empty returns FALSE
	{
		fprintf(stderr, "FAIL: Expected TRUE, got FALSE\n\n");
		exit(EXIT_FAILURE);
	}

	stack_kill(s);
}

/*
 * empty_test() - Test if stack_empty() returns a non-null pointer.
 * Preconditions: None.
 */
void empty_test()
{
	fprintf(stderr, "Testing if stack_empty returns NULL pointer...\n");

	stack *s = stack_empty(free);
	if (s == NULL)
	{
		fprintf(stderr, "FAIL: expected non-NULL, got NULL\n\n");
		exit(EXIT_FAILURE);
	}
	stack_kill(s);
}

/*
 * Function to run all tests. 
 */
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