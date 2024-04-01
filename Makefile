CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Source files
SRCS = mysh.c tokenizer.c arraylist.c

# Object files
OBJS = $(SRCS:.c=.o)

# Dependency files
DEPS = $(SRCS:.c=.d)

# Executable
TARGET = mysh

# Compilation rule
%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

# Dependency generation rule
%.d: %.c
    $(CC) -MM $(CFLAGS) $< > $@

# Include dependency files
-include $(DEPS)

# Build rule
$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) $^ -o $@

.PHONY: clean

clean:
    rm -f $(OBJS) $(DEPS) $(TARGET)
