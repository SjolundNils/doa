#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h> // for EXIT_FAILURE
#include <int_list.h>

// Macro to compute length of a C array
#define LEN(x) (sizeof(x)/sizeof(x[0]))

/*
 * Stub test program for the list implementation in list.c.
 *
 * Author: Niclas Borlin (niclas.borlin@cs.umu.se).
 *
 * Version information:
 *   2018-01-28: v1.0.  First public version.
 *   2019-01-23: v1.1.  Added file comment. Removed memory cleanup on failure.
 *   2023-01-21: v2.0.  Complete re-write. Semi-serious attempt at a complete test program for
 *                      int_list.
 *   2023-01-21: v2.01. Updated print identification command.
 *   2023-03-23: v2.1.  Renamed list_pos_are_equal to list_pos_is_equal.
 *   2023-03-23: v2.2.  Removed empty if statements in test code.
 */

#define VERSION "v2.2"
#define VERSION_DATE "2023-03-23"

/*
 * Function to compare the values stored in the list.
 */
bool value_equal(int v1, int v2)
{
        return v1 == v2;
}

/*
 * empty_returns_non_null() - Test that the list_empty() function returns a non-null pointer.
 * Precondition: None.
 */
void empty_returns_non_null(void)
{
        // Print a starting message
        fprintf(stderr,"Starting empty_returns_non_null()...");

        // Create an empty list
        list *l = list_empty();

        // l should be non-NULL
        if (l == NULL) {
                // Fail with error message
                fprintf(stderr, "FAIL: Expected a list pointer, got NULL.\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * empty_is_empty() - Test that the list_empty() list is empty.
 * Precondition: list_empty() returns non-null.
 */
void empty_is_empty(void)
{
        // Print a starting message
        fprintf(stderr,"Starting empty_is_empty()...");

        // Create an empty list
        list *l = list_empty();

        // The list returned by empty() should be is_empty()
        if (!list_is_empty(l)) {
                // Fail with error message
                fprintf(stderr, "FAIL: is_empty(empty()) == false, expected true\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * empty_first_end() - Test that first() == end() on an empty list.
 * Precondition: list_is_empty(l) == false.
 */
void empty_first_end(void)
{
        // Print a starting message
        fprintf(stderr,"Starting empty_first_end()...");

        // Create an empty list
        list *l = list_empty();

        // first(l) should be == end(l) for an empty list
        if (!(list_pos_is_equal(l, list_first(l), list_end(l)))) {
                // Fail with error message
                fprintf(stderr, "FAIL: expected first(l) == end(l), they are not\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/**
 * create_one_element_list() - Create a list with one element
 * @v: Value to insert
 *
 * Preconditions: list_empty() and list_first() works.
 *
 * Returns: A list with one element with the value v.
 *
 * The list is created by inserting an element at the first position
 * of an empty list.
 */
list *create_one_element_list(int v)
{
        // Create the list
        list *l = list_empty();
        // Insert an element at the only position in the empty list
        list_insert(l, v, list_first(l));

        return l;
}

/**
 * create_two_element_list() - Create a list with two elements
 * @v1: Value to insert first
 * @v2: Value to insert at the end
 *
 * Preconditions: list_empty(), list_first(), list_next(), list_insert() works.
 *
 * Returns: A list with two elements with the values v1 and v2 in that order.
 *
 * The the element with value v2 is inserted after the element with value v1.
 */
list *create_two_element_list(int v1, int v2)
{
        // Create the list
        list *l = list_empty();
        // Insert the first element
        list_pos p = list_insert(l, v1, list_first(l));
        // Insert the second element *after* the first
        list_insert(l, v2, list_next(l, p));

        return l;
}

/*
 * one_element_list_is_nonempty() - Test that a list with one element is not empty.
 * Precondition: list_empty() returns non-null.
 *               first(l) == end(l)
 */
void one_element_list_is_nonempty(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_is_nonempty()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        // One-element list should be non-empty
        if (list_is_empty(l)) {
                // Fail with error message
                fprintf(stderr, "FAIL: is_empty after insert == true, expected false\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * one_element_list_has_first_neq_end() - Test that a list with one element has first() != end().
 * Precondition: list_empty(), list_first() works
 */
void one_element_list_has_first_neq_end(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_has_first_neq_end()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        // One-element list should have first() != end()
        if (list_pos_is_equal(l, list_first(l), list_end(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: one-element list has first() == end()\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * insert_first_returns_correct_pos() - Test that insert(first()) returns the correct pos.
 * Precondition: list_empty(), list_first() works
 */
void insert_first_returns_correct_pos(void)
{
        // Print a starting message
        fprintf(stderr,"Starting insert_first_returns_correct_pos()...");

        // Create the list
        list *l = list_empty();
        // Insert an element with an arbitrary value
        list_pos p = list_insert(l, 24, list_first(l));

        // The returned position should be first in the modified list
        if (!list_pos_is_equal(l, p, list_first(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: position returned by insert() != first\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * inserted_element_has_correct_value() - Test that the inserted value can be recovered.
 * Precondition: list_empty(), list_first() works, list_insert() returns correct pos.
 */
void inserted_element_has_correct_value(void)
{
        // Print a starting message
        fprintf(stderr,"Starting inserted_element_has_correct_value()...");

        // Decide an arbitrary value to insert
        int val = 24;
        // Create a one-element list with the value
        list *l = create_one_element_list(24);

        // The value at the returned position should be the one we inserted
        int read_value = list_inspect(l, list_first(l));

        if (!value_equal(read_value, val)) {
                // Fail with error message
                fprintf(stderr, "FAIL: inspect returned %d, expected %d\n", read_value, val);
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * next_does_something() - Test that next returns another position than its argument.
 * Precondition: list_empty(), list_first() works,
 */
void next_does_something(void)
{
        // Print a starting message
        fprintf(stderr,"Starting next_does_something()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        list_pos p = list_first(l);

        // Next(p) should be != p
        if (list_pos_is_equal(l, list_next(l, p), p)) {
                // Fail with error message
                fprintf(stderr, "FAIL: expected next(p) != p\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * one_element_list_next_eq_end() - Test that next on first() in a one-element list returns end().
 * Precondition: list_empty(), list_first(), list_end() works
 */
void one_element_list_next_eq_end(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_next_eq_end()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        // Next(First()) should be == End(l)
        if (!list_pos_is_equal(l, list_next(l, list_first(l)), list_end(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: expected next(p) == end(l)\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * previous_does_something() - Test that previous returns another position than its argument.
 * Precondition: list_empty(), list_first(), list_end() works
 */
void previous_does_something(void)
{
        // Print a starting message
        fprintf(stderr,"Starting previous_does_something()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        list_pos p = list_end(l);

        // Previous(p) should be != p
        if (list_pos_is_equal(l, list_previous(l, p), p)) {
                // Fail with error message
                fprintf(stderr, "FAIL: expected previous(p) != p\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * one_element_list_previous_end_eq_first() - Test that previous on
 *     end() in a one-element list returns first().
 * Precondition: list_empty(), list_first(), list_end() works
 */
void one_element_list_previous_end_eq_first(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_previous_eq_end()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        // Previous(End()) should be == First(l)
        if (!list_pos_is_equal(l, list_previous(l, list_end(l)), list_first(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: expected previous(p) == end(l)\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/**
 * previous_is_inv_next() - Check if previous is inverse of next
 * @l: A list with a least one element
 * @p: A position, not equal to end
 *
 * Returns: True if Prev(l, Next(l, p)) == p
 */
bool previous_is_inv_next(const list *l, const list_pos p)
{
        return list_pos_is_equal(l, list_previous(l, list_next(l, p)), p);
}

/**
 * next_is_inv_previous() - Check if next is inverse of previous
 * @l: A list with a least one element
 * @p: A position, not equal to first
 *
 * Returns: True if Next(l, Prev(l, p)) == p
 */
bool next_is_inv_previous(const list *l, const list_pos p)
{
        return list_pos_is_equal(l, list_next(l, list_previous(l, p)), p);
}

/*
 * one_element_list_prev_is_inv_next() - Test that previous is inverse to next in  a
 *     one-element list.
 * Precondition: list_empty(), list_first(), list_end() works
 */
void one_element_list_prev_is_inv_next(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_prev_is_inv_next()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        // Check that previous is inverse of next on first()
        if (!previous_is_inv_next(l, list_first(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: prev(next()) failed on first() for one-element list\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * one_element_list_next_is_inv_prev() - Test that previous is inverse to next in  a
 *     one-element list.
 * Precondition: list_empty(), list_first(), list_end() works
 */
void one_element_list_next_is_inv_prev(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_next_is_inv_prev()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        // Check that next is inverse of previous on end()
        if (!next_is_inv_previous(l, list_end(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: next(prev()) failed on end() for one-element list\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * insert_remove_is_empty() - Test that insert followed by remove gives empty list.
 * Precondition: list_empty(), list_is_empty(), list_first(), list_end(), list_insert() works
 */
void insert_remove_is_empty(void)
{
        // Print a starting message
        fprintf(stderr,"Starting insert_remove_is_empty()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        // Remove only element
        list_remove(l, list_first(l));

        // Check that list is empty
        if (!list_is_empty(l)) {
                // Fail with error message
                fprintf(stderr, "FAIL: remove(insert()) list is non-empty\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * insert_remove_returns_end() - Test that insert followed by remove gives empty list.
 * Precondition: list_empty(), list_is_empty(), list_first(), list_end(), list_insert() works
 */
void insert_remove_returns_end(void)
{
        // Print a starting message
        fprintf(stderr,"Starting insert_remove_returns_end()...");

        // Create a one-element list storing an arbitrary value
        list *l = create_one_element_list(24);

        // Remove only element
        list_pos p = list_remove(l, list_first(l));

        // Check that remove(first()) on one-element list returns end()
        if (!list_pos_is_equal(l, list_end(l), p)) {
                // Fail with error message
                fprintf(stderr, "FAIL: remove(insert()) did not return end()\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * one_element_list_check_insert_first_pos() - Check return value of insert(first()) on
 *     one-element list
 *
 * Precondition: list_first(), list_end(), list_next(), list_prev(), works
 */
void one_element_list_check_insert_first_pos(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_check_insert_first_pos()...");

        // Create a two-element lists storing an arbitrary value
        list *l = create_one_element_list(24);

        list_pos p = list_insert(l, 30, list_first(l));

        // Returned position should be == first(l)
        if (!list_pos_is_equal(l, p, list_first(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: insert(first()) on one-element list did not "
                        "return correct position\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * one_element_list_check_insert_end_pos() - Check return value of insert(end()) on
 *     one-element list
 * Precondition: list_first(), list_end(), list_next(), list_prev(), works
 */
void one_element_list_check_insert_end_pos(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_check_insert_end_pos()...");

        // Create a two-element lists storing an arbitrary value
        list *l = create_one_element_list(24);

        list_pos p = list_insert(l, 30, list_end(l));

        // Returned position should be == prev(end(l))
        if (!list_pos_is_equal(l, p, list_previous(l, list_end(l)))) {
                // Fail with error message
                fprintf(stderr, "FAIL: insert(end()) on one-element list did not return "
                        "correct position\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * two_element_list_check_links() - Test next(first()) and prev(end()) are equal on
 *     two-element list
 * Precondition: list_first(), list_end(), list_next(), list_prev(), list_insert() works
 */
void two_element_list_check_links(void)
{
        // Print a starting message
        fprintf(stderr,"Starting two_element_list_check_links()...");

        // Create a two-element lists storing an arbitrary value
        list *l = create_two_element_list(24, 30);

        // Check prev(next()) on each valid position...

        // ...start with first
        if (!previous_is_inv_next(l, list_first(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: prev(next()) failed on first()\n");
                exit(EXIT_FAILURE);
        }

        // ...then middle
        if (!previous_is_inv_next(l, list_next(l, list_first(l)))) {
                // Fail with error message
                fprintf(stderr, "FAIL: prev(next()) failed on next(first())\n");
                exit(EXIT_FAILURE);
        }

        // Check next(prev()) on each valid position...

        // ...start with end
        if (!next_is_inv_previous(l, list_end(l))) {
                // Fail with error message
                fprintf(stderr, "FAIL: next(prev()) failed on end()\n");
                exit(EXIT_FAILURE);
        }

        // ...then middle
        if (!previous_is_inv_next(l, list_previous(l, list_end(l)))) {
                // Fail with error message
                fprintf(stderr, "FAIL: next(prev()) failed on previous(end())\n");
                exit(EXIT_FAILURE);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * one_element_list_insert_end() - Insert a value at end() of a one-element list and check
 *     the order
 * Precondition: list_first(), list_end(), list_next(), list_prev(), list_insert() works
 */
void one_element_list_insert_end(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_insert_end()...");

        // Numbers in the expected order
        int v[2] = { 11, 12 };

        // Create a one-element lists with 11
        list *l = create_one_element_list(v[0]);

        // Insert new element at end
        list_insert(l, v[1], list_end(l));

        // Check that we got the expected element order
        list_pos p = list_first(l);

        for (int i = 0; i < LEN(v); i++) {
                if (!value_equal(v[i], list_inspect(l, p))) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: expected %d, got %d after insert(end())\n",
                                v[i], list_inspect(l, p));
                        exit(EXIT_FAILURE);
                }
                p = list_next(l, p);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * one_element_list_insert_first() - Insert a value at first() of a one-element list and
 *     check the order
 * Precondition: list_first(), list_end(), list_next(), list_prev(), list_insert() works
 */
void one_element_list_insert_first(void)
{
        // Print a starting message
        fprintf(stderr,"Starting one_element_list_insert_first()...");

        // Numbers in the expected order
        int v[2] = { 11, 12 };

        // Create a one-element lists with 12
        list *l = create_one_element_list(v[1]);

        // Insert new element at first
        list_insert(l, v[0], list_first(l));

        // Check that we got the expected element order
        list_pos p = list_first(l);

        for (int i = 0; i < LEN(v); i++) {
                if (!value_equal(v[i], list_inspect(l, p))) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: expected %d, got %d after insert(first())\n",
                                v[i], list_inspect(l, p));
                        exit(EXIT_FAILURE);
                }
                p = list_next(l, p);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * two_element_list_insert_end() - Insert a value at end() of a two-element list and check
 *     the order
 * Precondition: list_first(), list_end(), list_next(), list_prev(), list_insert() works
 */
void two_element_list_insert_end(void)
{
        // Print a starting message
        fprintf(stderr,"Starting two_element_list_insert_end()...");

        // Numbers in the expected order
        int v[3] = { 11, 12, 13 };

        // Create a two-element lists with 11
        list *l = create_two_element_list(v[0],v[1]);

        // Insert new element at end
        list_insert(l, v[2], list_end(l));

        // Check that we got the expected element order
        list_pos p = list_first(l);

        for (int i = 0; i < LEN(v); i++) {
                if (!value_equal(v[i], list_inspect(l, p))) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: expected %d, got %d after insert(end())\n",
                                v[i], list_inspect(l, p));
                        exit(EXIT_FAILURE);
                }
                p = list_next(l, p);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * two_element_list_insert_first() - Insert a value at first() of a two-element list and
 *     check the order
 * Precondition: list_first(), list_end(), list_next(), list_prev(), list_insert() works
 */
void two_element_list_insert_first(void)
{
        // Print a starting message
        fprintf(stderr,"Starting two_element_list_insert_first()...");

        // Numbers in the expected order
        int v[3] = { 11, 12, 13 };

        // Create a two-element lists with 11
        list *l = create_two_element_list(v[1],v[2]);

        // Insert new element at first
        list_insert(l, v[0], list_first(l));

        // Check that we got the expected element order
        list_pos p = list_first(l);

        for (int i = 0; i < LEN(v); i++) {
                if (!value_equal(v[i], list_inspect(l, p))) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: expected %d, got %d after insert(first())\n",
                                v[i], list_inspect(l, p));
                        exit(EXIT_FAILURE);
                }
                p = list_next(l, p);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * two_element_list_insert_middle() - Insert a value in the middle of a two-element list
 *     and check the order
 * Precondition: list_first(), list_end(), list_next(), list_prev(), list_insert() works
 */
void two_element_list_insert_middle(void)
{
        // Print a starting message
        fprintf(stderr,"Starting two_element_list_insert_middle()...");

        // Numbers in the expected order
        int v[3] = { 11, 12, 13 };

        // Create a two-element lists with 11
        list *l = create_two_element_list(v[0],v[2]);

        // Insert new element at next(first())
        list_insert(l, v[1], list_next(l, list_first(l)));

        // Check that we got the expected element order
        list_pos p = list_first(l);

        for (int i = 0; i < LEN(v); i++) {
                if (!value_equal(v[i], list_inspect(l, p))) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: expected %d, got %d after insert(middle())\n",
                                v[i], list_inspect(l, p));
                        exit(EXIT_FAILURE);
                }
                p = list_next(l, p);
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * n_element_list_insert_end() - Insert a value at end() of a n-element list and check the order
 * Precondition: list_first(), list_end(), list_next(), list_prev(), list_insert() works
 */
void n_element_list_insert_end(void)
{
        // Print a starting message
        fprintf(stderr,"Starting n_element_list_insert_end()...");

        // Start with an empty list
        list *l = list_empty();

        // n is the number of elements in the list
        for (int n = 1; n <= 5; n++) {
                // insert the value n at the end of the list
                list_insert(l, n, list_end(l));

                // Check that the element values are in the expected order, 1, ..., n
                list_pos p = list_first(l);

                for (int i = 0; i < n; i++) {
                        if (!value_equal(i + 1, list_inspect(l, p))) {
                                // Fail with error message
                                fprintf(stderr, "FAIL: expected %d, got %d after insert(end())\n",
					i + 1, list_inspect(l, p));
                                exit(EXIT_FAILURE);
                        }
                        p = list_next(l, p);
                }
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * n_element_list_insert_first() - Insert a value at first() of a n-element list and check
 *     the order
 * Precondition: list_first(), list_end(), list_next(), list_prev(), list_insert() works
 */
void n_element_list_insert_first(void)
{
        // Print a starting message
        fprintf(stderr,"Starting n_element_list_insert_first()...");

        // Start with an empty list
        list *l = list_empty();

        // n is the number of elements in the list
        for (int n = 1; n <= 5; n++) {
                // insert the value n at the beginning of the list
                list_insert(l, n, list_first(l));

                // Check that the element values are in the expected order n, n-1, ..., 1
                list_pos p = list_first(l);

                for (int i = 0; i < n; i++) {
                        if (!value_equal(n - i, list_inspect(l, p))) {
                                // Fail with error message
                                fprintf(stderr, "FAIL: expected %d, got %d after "
					"insert(first())\n", n - i, list_inspect(l, p));
                                exit(EXIT_FAILURE);
                        }
                        p = list_next(l, p);
                }
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/**
 * count_list_elements_forwards() - Failsafely count the number of elements in a list from front()
 * @l: A list with any number of elements
 * @max: The maximum number of elements to visit
 *
 * Returns: The number of elements in the list if <= max, otherwise -1.
 *
 * Traverses the list with next() from front() until end() is
 * encountered. If more than max elements are encountered, bail out
 * and return -1.
 */
int count_list_elements_forwards(const list *l, int max)
{
        int n = 0;
        list_pos p = list_first(l);
        while (!list_pos_is_equal(l, p, list_end(l))) {
                n++;
                if (n > max) {
                        // We've gone past the limit, bail out
                        return -1;
                }
                p = list_next(l, p);
        }
        return n;
}

/**
 * count_list_elements_backwards() - Failsafely count the number of elements in a list from end()
 * @l: A list with any number of elements
 * @max: The maximum number of elements to visit
 *
 * Returns: The number of elements in the list if <= max, otherwise -1.
 *
 * Traverses the list with prev() from end() until first() is
 * encountered. If more than max elements are encountered, bail out
 * and return -1.
 */
int count_list_elements_backwards(const list *l, int max)
{
        int n = 0;
        list_pos p = list_end(l);
        while (!list_pos_is_equal(l, p, list_first(l))) {
                n++;
                if (n > max) {
                        // We've gone past the limit, bail out
                        return -1;
                }
                p = list_previous(l, p);
        }
        return n;
}

/*
 * insert_and_count_forwards() - Check element count from first() after repeated inserts
 * Precondition: list_empty(), list_first(), list_end(), list_next() works
 */
void insert_and_count_forwards(void)
{
        // Print a starting message
        fprintf(stderr,"Starting insert_and_count_forwards()...");

        // Start with empty list
        list *l = list_empty();

        for (int n=0; n<5; n++) {
                // Check the actual count.
                // Use max=10 as a safeguard against infinite loops
                int c = count_list_elements_forwards(l, 10);
                if (c != n) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: count_forwards returned %d elements, "
				"expected %d\n", c, n);
                        exit(EXIT_FAILURE);
                }
                // Insert one more element
                list_insert(l, n, list_first(l));
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * insert_and_count_backwards() - Check element count from end() after repeated inserts
 * Precondition: list_empty(), list_first(), list_end(), list_previous() works
 */
void insert_and_count_backwards(void)
{
        // Print a starting message
        fprintf(stderr,"Starting insert_and_count_backwards()...");

        // Start with empty list
        list *l = list_empty();

        for (int n=0; n<5; n++) {
                // Check the actual count.
                // Use max=10 as a safeguard against infinite loops
                int c = count_list_elements_backwards(l, 10);
                if (c != n) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: count_backwards returned %d elements, "
				"expected %d\n", c, n);
                        exit(EXIT_FAILURE);
                }
                // Insert one more element
                list_insert(l, n, list_first(l));
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * remove_and_count_forwards() - Check element count from first() after repeated removes
 * Precondition: list_empty(), list_first(), list_end(), list_next() works
 */
void remove_and_count_forwards(void)
{
        // Print a starting message
        fprintf(stderr,"Starting remove_and_count_forwards()...");

        // Create a five-element list
        list *l = list_empty();
        for (int i = 0; i < 5; i++) {
                list_insert(l, i, list_first(l));
        }

        for (int n=5; n>=0; n--) {
                // Check the actual count.
                // Use max=10 as a safeguard against infinite loops
                int c = count_list_elements_forwards(l, 10);
                if (c != n) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: count_forwards returned %d elements, "
				"expected %d\n", c, n);
                        exit(EXIT_FAILURE);
                }
                if (n > 0) {
                        // Remove one element unless the list is empty
                        list_remove(l, list_first(l));
                }
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

/*
 * remove_and_count_backwards() - Check element count from first() after repeated removes
 * Precondition: list_empty(), list_first(), list_end(), list_next() works
 */
void remove_and_count_backwards(void)
{
        // Print a starting message
        fprintf(stderr,"Starting remove_and_count_backwards()...");

        // Create a five-element list
        list *l = list_empty();
        for (int i = 0; i < 5; i++) {
                list_insert(l, i, list_first(l));
        }

        for (int n=5; n>=0; n--) {
                // Check the actual count.
                // Use max=10 as a safeguard against infinite loops
                int c = count_list_elements_backwards(l, 10);
                if (c != n) {
                        // Fail with error message
                        fprintf(stderr, "FAIL: count_backwards returned %d elements, "
				"expected %d\n", c, n);
                        exit(EXIT_FAILURE);
                }
                if (n > 0) {
                        // Remove one element unless the list is empty
                        list_remove(l, list_first(l));
                }
        }

        // Everything went well, clean up
        fprintf(stderr,"cleaning up...");
        list_kill(l);
        fprintf(stderr,"done.\n");
}

int main(void)
{
        printf("%s, %s %s: Test program for the typed int_list datatype.\n",
		__FILE__, VERSION, VERSION_DATE);
        printf("Code base version %s.\n\n", CODE_BASE_VERSION);

        empty_returns_non_null();
        empty_is_empty();
        empty_first_end();
        one_element_list_is_nonempty();
        one_element_list_has_first_neq_end();
        insert_first_returns_correct_pos();
        inserted_element_has_correct_value();
        next_does_something();
        one_element_list_next_eq_end();
        previous_does_something();
        one_element_list_previous_end_eq_first();
        one_element_list_prev_is_inv_next();
        one_element_list_next_is_inv_prev();
        insert_remove_is_empty();
        insert_remove_returns_end();
        one_element_list_check_insert_first_pos();
        one_element_list_check_insert_end_pos();
        two_element_list_check_links();
        one_element_list_insert_end();
        one_element_list_insert_first();
        insert_and_count_forwards();
        insert_and_count_backwards();
        remove_and_count_forwards();
        remove_and_count_backwards();
        two_element_list_insert_end();
        two_element_list_insert_first();
        two_element_list_insert_middle();
        n_element_list_insert_end();
        n_element_list_insert_first();

        fprintf(stderr,"\nSUCCESS: Implementation passed all tests. Normal exit.\n");
        return 0;
}
