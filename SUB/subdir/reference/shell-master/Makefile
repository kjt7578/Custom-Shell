CFLAGS=-Wall -g

all: shell

shell: shell.o
	gcc $(CFLAGS) -o shell shell.o

shell.o: shell.c shell.h
	gcc $(CFLAGS) -c shell.c

clean:
	rm -f shell shell.o
