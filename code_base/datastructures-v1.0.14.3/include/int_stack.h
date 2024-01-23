#ifndef __INT_STACK_H
#define __INT_STACK_H

#include <stdbool.h>
#include "util.h"

/*
 * Declaration of a generic stack for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The stack stores void pointers, so it can be used to
 * store all types of values. After use, the function stack_kill must
 * be called to de-allocate the dynamic memory used by the stack
 * itself. The de-allocation of any dynamic memory allocated for the
 * element values is the responsibility of the user of the stack,
 * unless a free_function is registered in stack_empty.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *	    Adam Dahlgren Lindstrom (dali@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0 2018-01-28: First public version.
 */

// ==========PUBLIC DATA TYPES============

// Stack type.
#define MAX_STACK_SIZE 100

// We must define the struct publically for the compiler to know its
// size for copying, etc.
typedef struct stack {
    int elements[MAX_STACK_SIZE];
    int first_free_pos;
} stack;

// ==========DATA STRUCTURE INTERFACE==========

/**
 * stack_empty() - Create an empty stack.
 *
 * Returns: A new stack.
 */
stack stack_empty(void);

/**
 * stack_is_empty() - Check if a stack is empty.
 * @s: Stack to check.
 *
 * Returns: True if stack is empty, otherwise false.
 */
bool stack_is_empty(const stack s);

/**
 * stack_push() - Push a value on top of a stack.
 * @s: Stack to manipulate.
 * @v: Value (integer) to be put on the stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack stack_push(stack s, int v);

/**
 * stack_pop() - Remove the element at the top of a stack.
 * @s: Stack to manipulate.
 *
 * NOTE: Undefined for an empty stack.
 *
 * Returns: The modified stack.
 *          NOTE: After the call, the input stack should be considered invalid.
 */
stack stack_pop(stack s);

/**
 * stack_top() - Inspect the value at the top of the stack.
 * @s: Stack to inspect.
 *
 * Returns: The value at the top of the stack.
 *	    NOTE: The return value is undefined for an empty stack.
 */
int stack_top(const stack s);

/**
 * stack_kill() - Destroy a given stack.
 * @s: Stack to destroy.
 *
 * Does nothing since the stack does not use any dynamic
 * memory. Included for symmetry with generic stack.h.
 *
 * Returns: Nothing.
 */
void stack_kill(stack s);

/**
 * stack_print() - Iterate over the stack elements and print their values.
 * @s: Stack to inspect.
 *
 * Iterates over the stack and prints each integer.
 *
 * Returns: Nothing.
 */
void stack_print(const stack s);

#endif
