CC     = mpicc
CFLAGS = -g -O2 -Wall -std=c99

all: reductions

reductions: reductions.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	-rm reductions

