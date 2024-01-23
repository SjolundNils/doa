#include <stdio.h>
#include <stdlib.h>

#include <int_stack.h>

/*
 * Implementation of an integer stack for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Author: Niclas Borlin (niclas@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se)
 *			     Niclas Borlin (niclas@cs.umu.se)
 *			     Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Version information:
 *	 2022-03-24: v1.0, first public version.
 */

// ===========INTERNAL DATA TYPES============

/**
 * stack_empty() - Create an empty stack.
 *
 * Returns: An empty stack.
 */
stack stack_empty(void)
{
	stack s;
	s.first_free_pos = 0;
	return s;
}

/**
 * stack_is_empty() - Check if a stack is empty.
 * @s: Stack to check.
 *
 * Returns: True if stack is empty, otherwise false.
 */
bool stack_is_empty(const stack s)
{
	return s.first_free_pos == 0;
}

/**
 * stack_push() - Push a value on top of a stack.
 * @s: Stack to manipulate.
 * @v: Value (integer) to be put on the stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack stack_push(stack s, int v)
{
	// Store value at first free position.
	s.elements[s.first_free_pos]=v;
	// Update first free position.
	s.first_free_pos++;

	// Return the new stack.
	return s;
}

/**
 * stack_pop() - Remove the element at the top of a stack.
 * @s: Stack to manipulate.
 *
 * NOTE: Undefined for an empty stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack stack_pop(stack s)
{
	if (stack_is_empty(s)) {
		fprintf(stderr, "stack_pop: Warning: pop on empty stack\n");
	} else {
		// We only have to decrease the first free position to
		// indicate that the element that used to be on top of the
		// stack is now free for use.
		s.first_free_pos--;
	}
	return s;
}

/**
 * stack_top() - Inspect the value at the top of the stack.
 * @s: Stack to inspect.
 *
 * Returns: The integer at the top of the stack.
 *		NOTE: The return value is undefined for an empty stack.
 */
int stack_top(const stack s)
{
	if (stack_is_empty(s)) {
		fprintf(stderr, "stack_top: Warning: top on empty stack\n");
	}
	return s.elements[s.first_free_pos-1];
}

/**
 * stack_kill() - Destroy a given stack.
 * @s: Stack to destroy.
 *
 * Does nothing since the stack does not use any dynamic
 * memory. Included for symmetry with generic stack.h.
 *
 * Returns: Nothing.
 */
void stack_kill(stack s)
{
	// Do nothing.
}

/**
 * stack_print() - Iterate over the stack elements and print their values.
 * @s: Stack to inspect.
 *
 * Iterates over the stack and prints each integer.
 *
 * Returns: Nothing.
 */
void stack_print(const stack s)
{
	printf("{ ");
	// Print elements from the top down.
	for (int i=s.first_free_pos-1; i>=0; i--) {
		printf("[%d]", s.elements[i]);
		if (i>0) {
			printf(", ");
		}
	}
	printf(" }\n");
}
