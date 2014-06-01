FLAGS = -g -O2 -Wall -Werror

CC = mpicc
FC = mpif90
LD = $(CC)

# BIGMPI_MAX_INT allow one to debug without requiring
# 4+ GiB of memory for every test.  This value must
# be set to something less than INT_MAX.
CPPFLAGS  = -I. -I../include -DBIGMPI_MAX_INT=1000000L
# Pedantic debug output that only developers should care about.
CPPFLAGS += -DBIGMPI_DEBUG
# Avoid using user-defined reduction operations.
#CPPFLAGS += -DBIGMPI_CLEAVER

# CHOOSE 1 OF THE FOLLOWING 3
# Implement v-collectives using Neighborhood_alltoallw
#CPPFLAGS += -DBIGMPI_VCOLLS_NBHD_ALLTOALLW
# Implement v-collectives using Send-Recv
CPPFLAGS += -DBIGMPI_VCOLLS_P2P
# Implement v-collectives using RMA
#CPPFLAGS += -DBIGMPI_VCOLLS_RMA

# Try to use MPI_TYPE_VECTOR if count not prime...
CPPFLAGS += -DBIGMPI_AVOID_TYPE_CREATE_STRUCT

CFLAGS = $(FLAGS) -std=c99 $(CPPFLAGS)
FFLAGS = $(FLAGS) -std=f95
LDFLAGS = $(CFLAGS)

LIBS =

.PHONY: all clean realclean install

LIBRARY  = libbigmpi.a
OBJECTS  = type_contiguous_x.o utils.o
OBJECTS += sendrecv_x.o reductions_x.o rma_x.o
OBJECTS += collectives_x.o vcollectives_x.o new_vcollectives_x.o neighborhood_collectives_x.o
TESTS    =

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

