FLAGS = -g -O2 -Wall

CC = mpicc
FC = mpif90
LD = $(CC)

CFLAGS = $(FLAGS) -std=c99
FFLAGS = $(FLAGS) -std=f95
LDFLAGS = 

.PHONY: all clean realclean

LIBRARY = libbigmpi.a
OBJECTS = 
TESTS   = 

all: $(LIBRARY)


$(LIBRARY): $(OBJECTS)
	-ar -r $(LIBRARY) $(OBJECTS)


%.o: %.c bigmpi.h bigmpi_impl.h
	$(CC) $(CFLAGS) $< -o $@


clean: 
	-rm -f $(OBJECTS)


realclean: clean 
	-rm -f $(TESTS) 
	-rm -f $(LIBRARY)


