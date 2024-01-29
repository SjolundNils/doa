#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

/*
 * Test program for the stack implementation in stack.c.
 *
 * Author: Mikael Meander (ens20mlr@cs.umu.se)
 *
 * Version information:
 *   2022-01-29: v1.0. First public version.
 */

/*
 * Axiom 1. Checks if the stack is empty. Prints an error message
 * to stderr and exits via exit() with an error if the test
 * fails.
 */
void empty_stack_test()
{
	stack *s=stack_empty(NULL);

	if (!stack_is_empty(s))
    {
		fprintf(stderr, "FAIL: New stack is not empty!\n");
		exit(EXIT_FAILURE);
	}

    stack_kill(s);
}

/*
 * Axiom 2. Checks if a element is inserted into an empty stack, the
 * stack is not empty. Prints an error message to stderr and exits via
 * exit() with an error signal if the test fails.
 */
void one_element_test()
{
	stack *s=stack_empty(NULL);

	int *v=malloc(sizeof(*v));
	*v=4;
	s=stack_push(s,v);
	if (stack_is_empty(s)) 
	{
		fprintf(stderr, "FAIL: Stack with one element is empty!\n");
		exit(EXIT_FAILURE);
	}        
	while (!stack_is_empty(s))
    {
		int *v=stack_top(s);
		s=stack_pop(s);
		free(v);
	}
	stack_kill(s);
}

/*
 *Axiom 3
 *A test to check that if you put a element into the stack and then removes 
 *the element on top of the stack, the stack is the same as before.
 */
void pop_stack_test()
{
    stack *s=stack_empty(NULL);
    //Create three elements and insert them to the stack on top of each other
    int*a=malloc(sizeof(*a));
    *a= 1;
    s=stack_push(s,a);
    
    int*b=malloc(sizeof(*b));
    *b=2;
    s=stack_push(s,b);
    
    int*c=malloc(sizeof(*c));
    *c=3;
    s=stack_push(s,c);
    
   
    int *t=stack_top(s);
    {
        t=stack_top(s);
        if(*c != *t)
        {
            printf("FAIL: Top is not what it should be");
            exit(EXIT_FAILURE);
        }
        else
        {
            s=stack_pop(s);
            t=stack_top(s);
            if (*b != *t)
            {   
                printf("FAIL: Top is not what it should be");
                exit(EXIT_FAILURE);
            }
            else
            {
                s=stack_pop(s);
                t=stack_top(s);
                if (*a != *t)
                {
                    printf("FAIL: Top is not what it should be");
                    exit(EXIT_FAILURE);
                }
            }
        }
    } 
     if (stack_is_empty(s))
    {
        fprintf(stderr, "FAIL: Stack is not empty");
		exit(EXIT_FAILURE);
    }
    free(a);
    free(b);
    free(c);
    stack_kill(s);
}

/*Axiom 4. Test if you can get the value on the top of a stack with top(push(v,s))*/
void element_value_test()
{
	stack *s=stack_empty(NULL);

	if(stack_is_empty(s))
	{
		for(int i=1; i<=4;i++)
		{ 
			int*v=malloc(sizeof(*v));
			*v=i;
			s=stack_push(s,v);
			int *s_top= stack_top(s);
			if(*s_top!= i)
			{
				fprintf(stderr, "FAIL: elements inserted in wrong order");
				exit(EXIT_FAILURE);

			}
			
		}
	}
	int *v=malloc(sizeof(*v));
	*v=4;
	s=stack_push(s,v);

	/*Verify that value 4 is at the top of the stack.
	*If it is not, print a error message to stderr and exits via 
	*exit() with an error signal if the test fails.
	*/
	int *s_top=stack_top(s);
	if (*s_top!=*v) 
    {
		fprintf(stderr, "FAIL: Wrong top in stack!");
		exit(EXIT_FAILURE);
	}
	while (!stack_is_empty(s))
	{
		int *v=stack_top(s);
		s=stack_pop(s);
		free(v);
	}
	stack_kill(s);

}




/*Axiom 5
 *Checks in a stack thats not empty. That if you remove the element on the top
 *and then puts it back, the stack will look like before
 */
void pop_top_test()
{
	stack *s=stack_empty(NULL);
	
		for(int i=0; i<=5;i++)
		{ 
			int*v=malloc(sizeof(*v));
			*v=i;
			s=stack_push(s,v);
		}
	
	for(int p=5; p>=0;p--)
	{ 
        
		int *inspect = stack_top(s);
		if (*inspect!=p)
		{
			fprintf(stderr, "FAIL: Top is not what it should be");
			exit(EXIT_FAILURE);
		}
		s= stack_pop(s);
        free (inspect);
	}

stack *s2=stack_empty(NULL);

if (stack_is_empty(s2))
{
    for(int i=0; i<=5;i++)
    { 
        int*v=malloc(sizeof(*v));
        *v=i;
        s2=stack_push(s2,v);  
        int *l = stack_top(s2);
        if (*l != i)
		{
			fprintf(stderr, "FAIL: Top is not what it should be");
			exit(EXIT_FAILURE);
		}
		
    }

	for(int p=5; p>=0;p--)
	{ 
       int *i=stack_top(s2);
       s2=stack_pop(s2);
       free(i);
    }
}
    if (!stack_is_empty(s))
    {
        fprintf(stderr, "FAIL: Stack is not empty");
		exit(EXIT_FAILURE);
    }
	stack_kill(s);
    stack_kill(s2);
}


int main (void)
{
  
	fprintf(stderr,"Running TEST1: empty_stack_test(): ");
	empty_stack_test();
	fprintf(stderr,"OK.\n");
	fprintf(stderr,"Running TEST2: one_element_test(): ");
	one_element_test();
	fprintf(stderr,"OK.\n");
	fprintf(stderr,"Running TEST3: pop_stack_test() : ");
	pop_stack_test();
	fprintf(stderr,"OK.\n");
    fprintf(stderr,"Running TEST4: element_value_test() : ");
	element_value_test();
	fprintf(stderr,"OK.\n");
	fprintf(stderr,"Running TEST5: pop_top_test(): ");
	pop_top_test();
	fprintf(stderr,"OK.\n");
	
	fprintf(stderr,"SUCCESS: Implementation passed all tests. Normal exit.\n");
	return 0;
}
