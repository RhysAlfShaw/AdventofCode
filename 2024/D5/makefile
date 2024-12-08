CC=gcc
CFLAGS=-Wall -Wextra

# Optimization targets
.PHONY: fast faster fastest unsafe

# Basic optimizations (-O2)
fast: CFLAGS += -O2 -march=native
fast: program

# More aggressive optimizations (-O3)
faster: CFLAGS += -O3 -march=native -flto
faster: program

# Maximum optimizations (potentially unsafe)
fastest: CFLAGS += -Ofast -march=native -flto -fno-signed-zeros \
    -fno-trapping-math -ffinite-math-only
fastest: program

# Build the program
program: main.c
	$(CC) $(CFLAGS) $^ -o $@

# Clean build files
clean:
	rm -f program

# Run the program
run: program
	$(CC) $(CFLAGS) main.c -o program
	./program
