from time import time
from itertools import product


def AdventOfCode():
    print("\n--- Advent of Code 2024 ---")


def evaluate_left_to_right(numbers, operators):
    result = numbers[0]
    for i, op in enumerate(operators):
        if op == "+":
            result += numbers[i + 1]
        elif op == "*":
            result *= numbers[i + 1]
        # part 2 concatenation operator
        elif op == "||":
            result = int(f"{result}{numbers[i + 1]}")
    return result


def process_equations(equations):
    total_calibration_result = 0

    for equation in equations:
        test_value, numbers = equation
        numbers = list(map(int, numbers))
        n = len(numbers)

        # permutations
        operator_permutations = product(["+", "*", "||"], repeat=n - 1)

        valid = False
        for ops in operator_permutations:
            if evaluate_left_to_right(numbers, ops) == test_value:
                valid = True
                break

        if valid:
            total_calibration_result += test_value

    return total_calibration_result


def parse_input(puzzle_input):
    equations = []
    for line in puzzle_input.strip().split("\n"):
        test_value, numbers = line.split(":")
        equations.append((int(test_value), numbers.strip().split()))
    return equations


def read_input(file):
    with open(file) as f:
        return f.read()


AdventOfCode()
start_time = time()

# file_path = "test_input.txt"
test_data = read_input("test_input.txt")
data = read_input("input.txt")
# print(data)
equations = parse_input(test_data)
result = process_equations(equations)
print(f"Total test result: {result}")
equations = parse_input(data)
result = process_equations(equations)
print(f"Result: {result}")

end_time = time()
print(f"Execution time: {end_time - start_time} seconds")
