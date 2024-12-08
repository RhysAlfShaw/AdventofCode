from time import time
from itertools import product


def AdventOfCode():
    print("\n--- Advent of Code 2024 ---")


def calculate_antinodes(grid):

    rows = len(grid)
    cols = len(grid[0])

    antennas = {}

    for r, row in enumerate(grid):
        for c, char in enumerate(row):
            if char != ".":
                if char not in antennas:
                    antennas[char] = []
                antennas[char].append((r, c))

    antinodes = set()

    for array in antennas.values():
        for i in range(len(array)):
            for j in range(i + 1, len(array)):
                r1, c1 = array[i]
                r2, c2 = array[j]
                antinodes.add((2 * r1 - r2, 2 * c1 - c2))
                antinodes.add((2 * r2 - r1, 2 * c2 - c1))

    return len([0 for r, c in antinodes if 0 <= r < rows and 0 <= c < cols])


def calculate_antinodes_part2(grid):

    rows = len(grid)
    cols = len(grid[0])

    antennas = {}

    for r, row in enumerate(grid):
        for c, char in enumerate(row):
            if char != ".":
                if char not in antennas:
                    antennas[char] = []
                antennas[char].append((r, c))

    antinodes = set()

    for array in antennas.values():
        for i in range(len(array)):
            for j in range(len(array)):
                if i == j:
                    continue
                r1, c1 = array[i]
                r2, c2 = array[j]
                dr = r2 - r1
                dc = c2 - c1
                r = r1
                c = c1
                while 0 <= r < rows and 0 <= c < cols:
                    antinodes.add((r, c))
                    r += dr
                    c += dc

    return len(antinodes)


def read_input(file):
    with open(file) as f:
        return f.read()


AdventOfCode()
t0 = time()
print("Test input, Part 1")
test_input = read_input("test_input.txt")
grid = test_input.strip().split("\n")
result = calculate_antinodes(grid)
print("Part 1: ", result)
result = calculate_antinodes_part2(grid)
print("Part 2: ", result)

print("\nReal input, Part 1")
real_input = read_input("input.txt")
grid = real_input.strip().split("\n")
result = calculate_antinodes(grid)
print("Part 1:", result)
result = calculate_antinodes_part2(grid)
print("Part 2:", result)
print("Time: {:.4f} ms".format((time() - t0) * 1000))
