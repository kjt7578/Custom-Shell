# Makefile

# Compiler and Compiler Flags
CC=gcc
CFLAGS=-Wall -g

# Objects
OBJS=arraylist.o mysh.o tokenizer.o

# Target
TARGET=mysh

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Source file dependencies
arraylist.o: arraylist.c arraylist.h
	$(CC) $(CFLAGS) -c arraylist.c

mysh.o: mysh.c mysh.h arraylist.h tokenizer.h
	$(CC) $(CFLAGS) -c mysh.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

# Clean
clean:
	rm -f $(OBJS) $(TARGET)
