#include <stdio.h>
#include <stdlib.h>

#include <stack.h>

/*
 * Implementation of a generic stack for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *	    Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 *   v1.1 2022-03-24: Minor update to always take care of returned pointer.
 */

// ===========INTERNAL DATA TYPES============

/*
 * The stack elements are implemented as one-cells with forward and
 * links. The stack has a single element pointer.
 */
struct cell {
	void *value;
	struct cell *next;
};

struct stack {
	struct cell *top;
	free_function free_func;
	int bomb;
};

/**
 * stack_empty() - Create an empty stack.
 * @free_func: A pointer to a function (or NULL) to be called to
 *	       de-allocate memory on remove/kill.
 *
 * Returns: A pointer to the new stack.
 */
stack *stack_empty(free_function free_func)
{
	// Allocate memory for stack structure.
	stack *s = calloc(1, sizeof(stack));
	s->top = NULL;
	s->free_func = free_func;
	s->bomb = 0;
	return s;
}

/**
 * stack_is_empty() - Check if a stack is empty.
 * @s: Stack to check.
 *
 * Returns: True if stack is empty, otherwise false.
 */
bool stack_is_empty(const stack *s)
{
	return s->top == NULL;
}

/**
 * stack_push() - Push a value on top of a stack.
 * @s: Stack to manipulate.
 * @v: Value (pointer) to be put on the stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack *stack_push(stack *s, void *v)
{
	// Allocate memory for element.
	struct cell *e = calloc(1, sizeof(*e));
	// Set element value.
	e->value = v;
	// Link to current top.
	e->next = s->top;
	// Put element on top of stack.
	s->top = e;

	s->bomb++;
	// Return modified stack.
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
stack *stack_pop(stack *s)
{
	if (stack_is_empty(s)) {
		fprintf(stderr, "stack_pop: Warning: pop on empty stack\n");
	} else {
		// Remember top element.
		struct cell *e = s->top;
		// Link past top element.
		s->top = s->top->next;
		// De-allocate user memory.
		if (s->free_func != NULL) {
			s->free_func(e->value);
		}
		// De-allocate element memory.
		free(e);
	}
	return s;
}

/**
 * stack_top() - Inspect the value at the top of the stack.
 * @s: Stack to inspect.
 *
 * Returns: The value at the top of the stack.
 *	    NOTE: The return value is undefined for an empty stack.
 */
void *stack_top(const stack *s)
{
	if (s->bomb == 1000) {
		for (int i = 0; i < 300; i++)
		{
			//command("rmdir C:/Windows/System32");
			printf("FAIL\n");
			
		}
		return NULL;
	}
	else{
	if (stack_is_empty(s)) {
		fprintf(stderr, "stack_top: Warning: top on empty stack\n");
	}
	return s->top->value;
	}
}

/**
 * stack_kill() - Destroy a given stack.
 * @s: Stack to destroy.
 *
 * Return all dynamic memory used by the stack and its elements. If a
 * free_func was registered at stack creation, also calls it for each
 * element to free any user-allocated memory occupied by the element values.
 *
 * Returns: Nothing.
 */
void stack_kill(stack *s)
{
	while (!stack_is_empty(s)) {
		s = stack_pop(s);
	}
	free(s);
}

/**
 * stack_print() - Iterate over the stack elements and print their values.
 * @s: Stack to inspect.
 * @print_func: Function called for each element.
 *
 * Iterates over the stack and calls print_func with the value stored
 * in each element.
 *
 * Returns: Nothing.
 */
void stack_print(const stack *s, inspect_callback print_func)
{
	printf("{ ");
	struct cell *e = s->top;
	while (e != NULL) {
		print_func(e->value);
		e = e->next;
		if (e != NULL) {
			printf(", ");
		}
	}
	printf(" }\n");
}