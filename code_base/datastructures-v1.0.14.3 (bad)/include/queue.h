#ifndef __QUEUE_H
#define __QUEUE_H

#include <stdbool.h>
#include "util.h"

/*
 * Declaration of a generic queue for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The queue stores void pointers, so it can be used to
 * store all types of values. After use, the function queue_kill must
 * be called to de-allocate the dynamic memory used by the queue
 * itself. The de-allocation of any dynamic memory allocated for the
 * element values is the responsibility of the user of the queue,
 * unless a free_function is registered in queue_empty.
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

// Queue type.
typedef struct queue queue;

// ==========DATA STRUCTURE INTERFACE==========

/**
 * queue_empty() - Create an empty queue.
 * @free_func: A pointer to a function (or NULL) to be called to
 *	       de-allocate memory on remove/kill.
 *
 * Returns: A pointer to the new queue.
 */
queue *queue_empty(free_function free_func);

/**
 * queue_is_empty() - Check if a queue is empty.
 * @q: Queue to check.
 *
 * Returns: True if queue is empty, otherwise false.
 */
bool queue_is_empty(const queue *q);

/**
 * queue_enqueue() - Put a value at the end of the queue.
 * @q: Queue to manipulate.
 * @v: Value (pointer) to be put in the queue.
 *
 * Returns: The modified queue.
 */
queue *queue_enqueue(queue *q, void *v);

/**
 * queue_dequeue() - Remove the element at the front of a queue.
 * @q: Queue to manipulate.
 *
 * NOTE: Undefined for an empty queue.
 *
 * Returns: The modified queue.
 */
queue *queue_dequeue(queue *q);

/**
 * queue_front() - Inspect the value at the front of the queue.
 * @q: Queue to inspect.
 *
 * Returns: The value at the top of the queue.
 *	    NOTE: The return value is undefined for an empty queue.
 */
void *queue_front(const queue *q);

/**
 * queue_kill() - Destroy a given queue.
 * @q: Queue to destroy.
 *
 * Return all dynamic memory used by the queue and its elements. If a
 * free_func was registered at queue creation, also calls it for each
 * element to free any user-allocated memory occupied by the element values.
 *
 * Returns: Nothing.
 */
void queue_kill(queue *q);

/**
 * queue_print() - Iterate over the queue elements and print their values.
 * @q: Queue to inspect.
 * @print_func: Function called for each element.
 *
 * Iterates over the queue and calls print_func with the value stored
 * in each element.
 *
 * Returns: Nothing.
 */
void queue_print(const queue *q, inspect_callback print_func);

#endif
