CC=gcc
LDFLAGS= -lrt
CFLAGS= -pedantic -std=c99 -Wall -g3 -O0 -m32
PROGRAM = program

all: $(PROGRAM)

$(PROGRAM): main.o
	$(CC) $(CFLAGS) main.o -o $(PROGRAM) $(LDFLAGS)

main.o: main.c

clean:
	rm -rf *.o
	rm -rf $(PROGRAM) core
