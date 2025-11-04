# === Makefile for Graph Matrix Project ===

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Target executable
TARGET = hw2

# Source and object files
SRCS = hw2.c graph_mat.c
OBJS = $(SRCS:.c=.o)
HEADERS = graph_mat.h

# Default rule — build everything
all: $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# Rule to compile each .c into .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Remove build artifacts
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).zip

# Package only the executable into a zip archive
package: $(TARGET)
	zip $(TARGET).zip $(TARGET)

# Declare phony targets
.PHONY: all clean run package
