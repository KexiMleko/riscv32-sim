CC := gcc
CFLAGS := -Wall -Wextra -g
TARGET := rvsim

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(TARGET)
