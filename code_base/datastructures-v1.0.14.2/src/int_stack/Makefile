MWE = int_stack_mwe1

SRC = int_stack.c
OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -std=c99 -Wall -I../../include -g

all:	mwe

# Minimum working examples.
mwe:	$(MWE)

# Object file for library
obj:	$(OBJ)

# Clean up
clean:
	-rm -f $(MWE) $(OBJ)

int_stack_mwe1: int_stack_mwe1.c int_stack.c
	gcc -o $@ $(CFLAGS) $^

memtest1: int_stack_mwe1
	valgrind --leak-check=full --show-reachable=yes ./$<
