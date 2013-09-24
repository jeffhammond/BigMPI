FLAGS = -g -O2 -Wall

CC = mpicc
CFLAGS = $(FLAGS) -std=c99

.PHONY: all clean realclean

TESTS =  aint_overflow.x

all: $(TESTS)


%.x: %.c
	$(CC) $(CFLAGS) $< -o $@


clean: 
	-rm -f *.o


realclean: clean 
	-rm -f $(TESTS) 


