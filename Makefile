CC       := gcc
SRCS     := $(shell find src -name '*.c')

OBJS     := $(patsubst src/%.c, build/%.o, $(SRCS))

DEPS     := $(OBJS:.o=.d)

INCLUDES := $(shell find src -type d | sed 's/^/-I/')
CFLAGS   := -Wall -Wextra -g $(INCLUDES)
TARGET   := build/rvsim

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(DEPS)

test: $(TARGET)
	./$(TARGET) ./tests/data/src/$(TSRC)

clean:
	rm -rf build
