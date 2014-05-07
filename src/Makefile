FLAGS = -g -O2 -Wall

CC = mpicc
FC = mpif90
LD = $(CC)

# BIGMPI_MAX_INT allow one to debug without requiring
# 4+ GiB of memory for every test.  This value must
# be set to something less than INT_MAX.
CPPFLAGS  = -I. -I../include -DBIGMPI_MAX_INT=1000000L
# Pedantic debug output that only developers should care about.
#CPPFLAGS += -DBIGMPI_DEBUG
# Avoid using user-defined reduction operations.
CPPFLAGS += -DBIGMPI_CLEAVER

CFLAGS = $(FLAGS) -std=c99 $(CPPFLAGS)
FFLAGS = $(FLAGS) -std=f95
LDFLAGS = $(CFLAGS)

LIBS =

.PHONY: all clean realclean install

LIBRARY = libbigmpi.a
OBJECTS = type_contiguous_x.o sendrecv_x.o collectives_x.o reductions_x.o rma_x.o
TESTS   =

all: $(LIBRARY)


$(LIBRARY): $(OBJECTS)
	-ar -r $(LIBRARY) $(OBJECTS)

%.x: %.o
	$(LD) $(LDFLAGS) $< $(LIBS) -o $@

%.o: %.c bigmpi_impl.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJECTS)

realclean: clean
	-rm -f $(LIBRARY)

install: libbigmpi.a
	-mkdir -p ../lib/.
	-cp $(LIBRARY) ../lib/.

