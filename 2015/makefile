CC = gcc
CFLAGS = -Wall -Wextra -g
BIN_DIR = bin

SRCS = $(wildcard *.c)
PROGS = $(patsubst %.c,$(BIN_DIR)/%,$(SRCS))

.PHONY: all clean run help

all: $(PROGS)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BIN_DIR)/%: %.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $<

run: all
	@for prog in $(PROGS); do \
		echo "-----------------------------------"; \
		echo "Running $$prog..."; \
		./$$prog; \
	done

run-%: $(BIN_DIR)/%
	@echo "Running $<..."
	@./$<

clean:
	rm -rf $(BIN_DIR) *.o