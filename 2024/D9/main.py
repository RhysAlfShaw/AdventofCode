from tqdm import tqdm
import time


def AdventOfCode():
    print("\n--- Advent of Code 2024 ---")


def read_input(path: str) -> str:
    return open(path).read().strip()


def sort_blocks(Data, part2: bool = False) -> int:
    array = []
    dots = []
    file_id = 0
    result = []
    pos = 0

    # interate though the input D. (each char)
    for i, c in enumerate(Data):
        if i % 2 == 0:  # if even
            count = int(c)
            if part2:
                # create whole "file"
                array.append((pos, count, file_id))
            for _ in range(count):
                result.append(file_id)
                if not part2:
                    array.append((pos, 1, file_id))
                pos += 1
            file_id += 1

        else:  # if odd
            count = int(c)
            dots.append((pos, count))
            for _ in range(count):
                result.append(None)
                pos += 1

    for pos, size, file_id in tqdm(reversed(array), desc="A", total=len(array)):
        for space_i, (space_pos, space_sz) in enumerate(dots):
            if space_pos < pos and size <= space_sz:
                for i in range(size):
                    assert result[pos + i] == file_id, f"{result[pos+i]=}"
                    result[pos + i] = None
                    result[space_pos + i] = file_id
                dots[space_i] = (space_pos + size, space_sz - size)
                break

    return result


def calculate_checksum(results):
    checksum = 0
    for i, c in enumerate(results):
        if c is not None:
            checksum += i * c
    return checksum


AdventOfCode()
Data = read_input("input.txt")
t0 = time.time()
sorted_blocks = sort_blocks(Data, False)
sorted_blocks_part2 = sort_blocks(Data, True)

part1 = calculate_checksum(sorted_blocks)
part2 = calculate_checksum(sorted_blocks_part2)
t1 = time.time()
print(f"Part_1: {part1}")
print(f"Part_2: {part2}")
print(f"Execution time: {t1 - t0:.4f} sec")
print("Done")
