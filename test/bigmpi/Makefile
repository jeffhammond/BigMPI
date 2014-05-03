FLAGS = -g -O2 -Wall

CC = mpicc
CPPFLAGS = -I../../include -DBIGMPI_MAX_INT=1000000
CFLAGS = $(FLAGS) -std=c99 $(CPPFLAGS)

LD = $(CC)
LDFLAGS = $(CFLAGS)

LIBS = -L../../lib -lbigmpi verify_buffer.o

.PHONY: all clean realclean test

TESTS = test_sendrecv_x.x test_isendrecv_x.x test_send_recv_x.x

all: $(TESTS)

test: $(TESTS)
	for i in $(TESTS); do mpirun -np 2 $$i; done
	for i in $(TESTS); do mpirun -np 2 $$i 1000 0; done

%.x: %.o verify_buffer.o
	$(LD) $(LDFLAGS) $< $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f *.o

realclean: clean
	-rm -f $(TESTS)

