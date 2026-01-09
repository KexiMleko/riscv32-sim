CC := gcc
CFLAGS := -Wall -Wextra -g -I./src
TARGET := build/rvsim

SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c,build/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

TSRC ?=branch.s

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

test: $(TARGET)
ifndef TSRC
	$(error TSRC variable is not set. Usage: make run TSRC=test.s)
endif
	./$(TARGET) ./tests/data/src/$(FSRC)

clean:
	rm -rf build/*
