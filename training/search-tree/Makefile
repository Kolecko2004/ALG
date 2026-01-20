CC = gcc

CFLAGS  = -Wall -Wextra -std=c11 -g
LDFLAGS = -lm

TARGET = 1st
SRCS   = 1st.c
OBJS   = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).zip

package: $(TARGET)
	zip $(TARGET).zip $(TARGET)

.PHONY: all clean run package
