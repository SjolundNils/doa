#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <int_stack.h>
#define n 5
/*
 * Test program for the stack implementation in stack.c
 * 
 * Author: Nils Sjölund (id23nsd@cs.umu.se)
 * 
 * Version information:
 * 	2023-02-01 v1.0. First public version
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
 * expected value with the top value, using stack_pop, after using pop() on the stack
 * 
 * Preconditions: stack_empty() works.
 */
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

/*
 * is_empty_returns_true_test() - Test if stack_is_empty returns TRUE
 * on a non-empty stack.
 * 
 * Preconditions: stack_empty() works.
 */
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

/*
 * is_empty_false_test() - Test if stack_is_empty returns FALSE
 * on a empty stack.
 * 
 * Preconditions: stack_empty() works.
 */
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

/*
 * Function to run all tests. 
 */
sdajsdksajdlsajdlksajdaslkd
asdjsadksa;
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