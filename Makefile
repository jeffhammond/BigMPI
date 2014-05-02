FLAGS = -g -O2 -Wall

CC = mpicc
FC = mpif90
LD = $(CC)

# BIGMPI_MAX_INT allow one to debug without requiring
# 4+ GiB of memory for every test.  This value must
# be set to something less than INT_MAX.
CFLAGS = $(FLAGS) -std=c99 -DBIGMPI_MAX_INT=1000000
FFLAGS = $(FLAGS) -std=f95
LDFLAGS = $(CFLAGS)

LIBS =

.PHONY: all clean realclean

LIBRARY = libbigmpi.a
OBJECTS = type_contiguous_x.o sendrecv_x.o
TESTS   =

all: $(LIBRARY)


$(LIBRARY): $(OBJECTS) bigmpi.h
	-ar -r $(LIBRARY) $(OBJECTS)

%.x: %.o bigmpi.h
	$(LD) $(LDFLAGS) $< $(LIBS) -o $@

%.o: %.c bigmpi.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJECTS)


realclean: clean
	-rm -f $(TESTS)
	-rm -f $(LIBRARY)


