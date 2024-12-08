from time import time


def AdventOfCode():
    print("\n--- Advent of Code 2024 ---")


def loadInput(isTest=True):
    global startTime

    if isTest:
        filename = f"test_input.txt"
    else:
        filename = f"input.txt"

    with open(filename) as f:
        content = [l[:-1] if l[-1] == "\n" else l for l in f.readlines()]

    return content


def printTimeTaken():
    global startTime
    endTime = time()
    print("Time: {:.3f}s".format(endTime - startTime))


def createGrid(isTest=True):
    grid = loadInput(isTest)

    for r in range(len(grid)):
        grid[r] = [g for g in grid[r]]

    if isTest:
        print("\nStart-GRID")
        showGrid(grid)
        print()

    for r in range(len(grid)):
        grid[r] = [grid[r][c] for c in range(len(grid[r]))]
    return grid


def doWalk(grid, startPos, d, isTest, part=1):
    pos = startPos
    visited = {(pos, d): 1}
    countLoop = 0
    height = len(grid)
    width = len(grid[0])
    grid[startPos[0]][startPos[1]] = "."

    while True:
        newPos = (pos[0] + directions[d][0], pos[1] + directions[d][1])

        if newPos[0] in [-1, width] or newPos[1] in [-1, height]:
            if isTest and part == 1:
                print()
                grid[pos[0]][pos[1]] = directions[d][2]
                if isTest:
                    showGrid(grid)
                grid[pos[0]][pos[1]] = "."
            break

        elif grid[newPos[0]][newPos[1]] == "#":
            if isTest and part == 1:
                print()
                grid[pos[0]][pos[1]] = directions[d][2]
                if isTest:
                    showGrid(grid)
                grid[pos[0]][pos[1]] = "."

            d = (d + 1) % 4

        else:
            pos = (pos[0] + directions[d][0], pos[1] + directions[d][1])

            if (pos, d) not in visited:
                if (pos, d) in visited:
                    visited[(pos, d)] += 1
                else:
                    visited[(pos, d)] = 1

            else:
                countLoop += 1
                break

    return visited, countLoop


def showVisitedGrid(grid, pointsOfWay):
    print()
    for p in pointsOfWay:
        grid[p[0]][p[1]] = "X"
    for r in range(len(grid)):
        print("".join(grid[r]))
    for p in pointsOfWay:
        grid[p[0]][p[1]] = "."
    print()


def showGrid(grid):
    print()
    for r in range(len(grid)):
        print("".join(grid[r]))
    print()


def doPart1(grid, startPos, isTest=True):
    if isTest:
        print("Startposition:", startPos)
        print("Startdirection:", directions[0][2], startDirection)
    visited, _ = doWalk(grid, startPos, startDirection, isTest)
    visited = {p for p, _ in visited}

    if isTest:
        print(
            "\n\nPositions visited by the guard before leaving the area - marked with an X:"
        )
        showVisitedGrid(grid, visited)
        print()

    result = len(visited)

    return result


def doPart2(grid, startPos, isTest=True):
    if isTest:
        print("Startposition:", startPos)
        print("Startdirection:", directions[0][2])
    visited, _ = doWalk(grid, startPos, startDirection, isTest, 2)

    result = 0

    if isTest:
        print()
        print("Additional rock for loop is marked with 0")

    for vpos in {p for p, _ in visited if p != startPos}:
        grid[vpos[0]][vpos[1]] = "#"
        tmp_result = doWalk(grid, startPos, startDirection, isTest, 2)[1]
        if tmp_result == 1:
            result += 1
            grid[startPos[0]][startPos[1]] = directions[startDirection][2]
            grid[vpos[0]][vpos[1]] = "0"
            if isTest:
                showGrid(grid)
        grid[vpos[0]][vpos[1]] = "."

    return result


startTime = time()


directions = {
    0: (-1, 0, "^"),  # up
    1: (0, 1, ">"),  # right
    2: (1, 0, "v"),  # down
    3: (0, -1, "<"),  # left
}

startDirection = 0

grid_t = createGrid(True)

startPos_t = [
    (x, y)
    for x in range(len(grid_t))
    for y in range(len(grid_t[0]))
    if grid_t[x][y] == "^"
][0]

grid = createGrid(False)

startPos = [
    (x, y) for x in range(len(grid)) for y in range(len(grid[0])) if grid[x][y] == "^"
][0]

AdventOfCode()

print(f"Solution Example: {doPart1(grid_t, startPos_t)}")
print(f"Solution Part 1:  {doPart1(grid, startPos, False)}")
print(f"Solution Example: {doPart2(grid_t, startPos_t)}")
print(f"Solution Part 2:  {doPart2(grid, startPos, False)}")
printTimeTaken()
