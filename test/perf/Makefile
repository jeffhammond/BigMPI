CC     = mpicc
CFLAGS = -g -O2 -Wall -std=c99

TESTS = reductions typecontig

all: $(TESTS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm -f $(TESTS)

