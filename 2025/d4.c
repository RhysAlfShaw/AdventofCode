#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ROWS 2048
#define MAX_COLS 2048

char grid[MAX_ROWS][MAX_COLS];
bool to_remove[MAX_ROWS][MAX_COLS];

int dr[] = {-1, -1, -1,  0, 0,  1, 1, 1};
int dc[] = {-1,  0,  1, -1, 1, -1, 0, 1};

int count_neighbors(int r, int c, int rows, int cols) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        int nr = r + dr[i];
        int nc = c + dc[i];

        if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
            if (grid[nr][nc] == '@') {
                count++;
            }
        }
    }
    return count;
}

int main() {
    printf("\n===================================\n");
    printf("     Advent of Code Day 4 Results  \n");
    printf("===================================\n");

    FILE *fp = fopen("input4.txt", "r");
    if (!fp) {
        perror("Error opening input4.txt");
        return 1;
    }

    int rows = 0;
    int cols = 0;
    char line[MAX_COLS + 2]; 

    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;
        
        if (strlen(line) == 0) continue;

        if (rows < MAX_ROWS) {
            strcpy(grid[rows], line);
            if (cols == 0) cols = strlen(line);
            rows++;
        }
    }
    fclose(fp);
    
    printf("Grid Size: %d x %d\n", rows, cols);

    long part1_ans = 0;
    long part2_ans = 0; 
    int iteration = 0;
    bool changes_made = true;

    while (changes_made) {
        changes_made = false;
        long round_removals = 0;
        iteration++;

        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                to_remove[r][c] = false; 
                
                if (grid[r][c] == '@') {
                    int neighbors = count_neighbors(r, c, rows, cols);
                    
                    if (neighbors < 4) {
                        to_remove[r][c] = true;
                        changes_made = true;
                        round_removals++;
                    }
                }
            }
        }

        if (changes_made) {
            if (iteration == 1) {
                part1_ans = round_removals;
            }

            part2_ans += round_removals;

            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    if (to_remove[r][c]) {
                        grid[r][c] = '.'; 
                    }
                }
            }
        }
    }

    printf("\n--------------------------------\n");
    printf("Part 1 Answer: %ld\n", part1_ans);
    printf("Part 2 Answer: %ld\n", part2_ans);
    printf("--------------------------------\n");

    return 0;
}