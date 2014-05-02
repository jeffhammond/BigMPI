FLAGS = -g -O2 -Wall

CC = mpicc
CPPFLAGS = -I../../include -DBIGMPI_MAX_INT=1000000
CFLAGS = $(FLAGS) -std=c99 $(CPPFLAGS)

LD = $(CC)
LDFLAGS = $(CFLAGS)

LIBS = -L../../lib -lbigmpi

.PHONY: all clean realclean

TESTS = test_sendrecv_x.x test_isendrecv_x.x

all: $(TESTS)

%.x: %.o
	$(LD) $(LDFLAGS) $< $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o

realclean: clean
	-rm -f $(TESTS)

