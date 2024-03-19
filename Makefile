CC = gcc
CFLAGS = -std=c11 -I./headers -pedantic -W -Wall -Wextra -g2 -ggdb
OUT_DIR = ./build

SRCS := $(wildcard src/*.c)
OBJS := $(SRCS:src/%.c=$(OUT_DIR)/%.o)

.PHONY: all clean

all: $(OUT_DIR)/output

$(OUT_DIR)/output: $(OBJS)
	@$(CC) $(CFLAGS) $^ -o $@ -lm -lncurses

$(OUT_DIR)/%.o: src/%.c | $(OUT_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ -lm -lncurses

$(OUT_DIR):
	@mkdir -p $(OUT_DIR)

clean:
	@rm -rf $(OUT_DIR)