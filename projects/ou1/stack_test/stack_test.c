#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stack.h>
#define n 5
/*
 * Test program for the stack implementation in stack.c
 * 
 * Author: Nils Sjölund (id23nsd@cs.umu.se)
 * 
 * Version information:
 * 	2023-01-29 v1.0. First public version
 */

/*
 * Function to compare the values stored in the stack. 
 */
bool value_equal(int v1, int v2)
{
	return v1 == v2;
}

/*
 * Function to de-allocate memory of elements stored in the stack.
 * @s: Stack to empty and de-allocate
 */
void free_stack(stack *s) 
{
	while(!stack_is_empty(s))
	{
		int *v = stack_top(s);
		free(v);
		s = stack_pop(s);
	}
	stack_kill(s);
}

/*
 * decreasing_loop_test() - Test a filled stack by comparing an 
 * expected value with the top value, using stack_pop, after using pop() on the stack
 * 
 * Preconditions: stack_empty() works.
 */
void decreasing_loop_test(void)
{
	fprintf(stderr, "Testing stack with decreasing values...\n");
	
	stack *s = stack_empty(NULL);
	
	int expected_value = n - 1;
	
	for (int i = 0; i < n; i++)
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
			exit(EXIT_FAILURE);
		}
		
		free(s_top);
		s = stack_pop(s);
		expected_value--;
	}
	
	free_stack(s);
}


/*
 * increasing_loop_test() - Test a stack by filling it using stack_push() 
 * and simultaneously comparing the expected value with the top value, 
 * using stack_top.
 * 
 * Preconditions: stack_empty() works.
 */
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

	free_stack(s);
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

	stack *s = stack_push(stack_empty(NULL), v);

	if (stack_is_empty(s)) // If is_empty returns TRUE
	{
		fprintf(stderr, "FAILED: Expected FALSE, got %s\n", stack_is_empty(s) ? "TRUE" : "FALSE");
		exit(EXIT_FAILURE);
	}

	free_stack(s);
}

/*
 * is_empty_false_test() - Test if stack_is_empty returns FALSE
 * on a empty stack.
 * 
 * Preconditions: stack_empty() works.
 */
void is_empty_false_test(void)
{
	fprintf(stderr, "Testing if is_empty returns false on empty stack...\n");
	stack *s = stack_empty(NULL);
	
	if (!stack_is_empty(s)) // If is_empty returns FALSE
	{
		fprintf(stderr, "FAILED: Expected TRUE, got %s\n", stack_is_empty(s) ? "TRUE" : "FALSE");
		exit(EXIT_FAILURE);
	}

	free_stack(s);
}

/*
 * empty_test() - Test that the stack_empty() function returns a non-null pointer.
 * Preconditions: None.
 */
void empty_test()
{
	fprintf(stderr, "Testing if stack_empty returns NULL pointer...\n");

	stack *s = stack_empty(NULL);
	if (s == NULL)
	{
		fprintf(stderr, "FAIL: expected non-NULL, got NULL\n");
		exit(EXIT_FAILURE);
	}

	free_stack(s);
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