#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>


void AdventCode2024() {
    printf("Advent Code 2024 Test! \n");
}


void StartTimer(struct timespec *start) {
    clock_gettime(CLOCK_MONOTONIC, start);
}


void EndTimer(struct timespec *start, struct timespec *end) {

    clock_gettime(CLOCK_MONOTONIC, end);
    double seconds = (end->tv_sec - start->tv_sec) +
                    (end->tv_nsec - start->tv_nsec) / 1000000000.0;
    printf("Time for execution: %f seconds\n", seconds);

}



typedef struct {
    char **content;
    int contentLength;
    int contentWidth;
} Crossword;



// open file and save contents to 2d chararacter array
Crossword ReadFile(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    char **content = NULL;
    int lines = 0;
    char buffer[1024]; 
    
    while (fgets(buffer, sizeof(buffer), file)) {
        content = realloc(content, (lines + 1) * sizeof(char *));
        content[lines] = malloc(strlen(buffer) + 1);
        strcpy(content[lines], buffer);
        lines++;
    }
    
    fclose(file);
    Crossword crossword;
    crossword.content = content;
    crossword.contentLength = lines;
    crossword.contentWidth = strlen(content[0]) - 1;
    return crossword;
}

typedef struct {
    int* x;
    int* y;
    int length;
} Position;

int FindNumberofXMAS(Crossword crossword) {
    int* x = NULL;
    int* y = NULL;
    int count = 0;

    // search for all 'X' positions
    for (int i = 0; i < crossword.contentLength; i++) {
        for (int j = 0; j < crossword.contentWidth; j++) {
            if (crossword.content[i][j] == 'X') {
                int* temp_x = realloc(x, (count + 1) * sizeof(int));
                int* temp_y = realloc(y, (count + 1) * sizeof(int));

                if (temp_x == NULL || temp_y == NULL) {
                    free(x);
                    free(y);
                    fprintf(stderr, "Memory allocation failed\n");
                    return -1;
                }

                x = temp_x;
                y = temp_y;

                x[count] = i;
                y[count] = j;
                count++;
            }
        }
    }

    int directions[8][2] = {
        {0, 1},   // right
        {0, -1},  // left
        {1, 0},   // down
        {-1, 0},  // up
        {1, 1},   // down-right
        {-1, -1}, // up-left
        {1, -1},  // down-left
        {-1, 1}   // up-right
    };

    // check for "XMAS" from each 'X' position

    int totalCount = 0;
    char word[] = "XMAS";

    for (int i = 0; i < count; i++) {
        int xi = x[i];
        int yi = y[i];

        // try all directions
        for (int dir = 0; dir < 8; dir++) {
            int dx = directions[dir][0];
            int dy = directions[dir][1];

            int found = 1; // assume we found the word, and check if every letter matches
            for (int k = 0; k < 4; k++) { // word length is 4 (XMAS)
                int nx = xi + dx * k;
                int ny = yi + dy * k;

                // check if the new position is within bounds
                if (nx < 0 || nx >= crossword.contentLength || ny < 0 || ny >= crossword.contentWidth) {
                    found = 0;
                    break;
                }

                // check if the character matches
                if (crossword.content[nx][ny] != word[k]) {
                    found = 0;
                    break;
                }
            }

            // if the word was found in this direction, increment the count
            if (found) {
                totalCount++;
            }
        }
    }

    free(x);
    free(y);

    return totalCount;
}


int FindNumberofPattern(Crossword crossword) {
    int count = 0;

    int* x = NULL;
    int* y = NULL;

    // search for all 'A' positions, but not edge positions
    for (int i = 1; i < crossword.contentLength - 1; i++) {
        for (int j = 1; j < crossword.contentWidth - 1; j++) {
            if (crossword.content[i][j] == 'A') {  // this is the anchor point for the pattern
                int* temp_x = realloc(x, (count + 1) * sizeof(int));
                int* temp_y = realloc(y, (count + 1) * sizeof(int));

                if (temp_x == NULL || temp_y == NULL) {
                    free(x);
                    free(y);
                    fprintf(stderr, "Memory allocation failed\n");
                    return -1;
                }

                x = temp_x;
                y = temp_y;

                x[count] = i;
                y[count] = j;
                count++;
            }
        }
    }


    // directions for checking: (dx, dy)
    // int directions[4][2] = {
    //     {1, 1},   // Down-right
    //     {-1, -1}, // Up-left
    //     {1, -1},  // Down-left
    //     {-1, 1}   // Up-right
    // };

    int totalCount = 0;


    char pattern1[] = {'M', 'M', 'S', 'S'}; // up right, up left, down right, down left
    char pattern2[] = {'S', 'S', 'M', 'M'};
    char pattern3[] = {'M', 'S', 'M', 'S'};
    char pattern4[] = {'S', 'M', 'S', 'M'};

    // for each A, check if any of the patterns are found in any of the directions
    for (int i = 0; i < count; i++) {
        int xi = x[i];
        int yi = y[i];
        
        // what are its down right, up left, down left, up right characters
        char down_right = crossword.content[xi + 1][yi + 1];
        
        char up_left = crossword.content[xi - 1][yi - 1];
        char down_left = crossword.content[xi + 1][yi - 1];
        char up_right = crossword.content[xi - 1][yi + 1];

        // check for pattern 1
        if (up_right == pattern1[0] && up_left == pattern1[1] && down_right == pattern1[2] && down_left == pattern1[3]) {
            totalCount++;
            // printf("Pattern 1 found\n");
            // location of the pattern
            // printf("Pattern location: %d, %d\n", xi, yi);
        }

        // check for pattern 2
        if (up_right == pattern2[0] && up_left == pattern2[1] && down_right == pattern2[2] && down_left == pattern2[3]) {
            totalCount++;
            // printf("Pattern 2 found\n");
            // location of the pattern
            // printf("Pattern location: %d, %d\n", xi, yi);
        }

      

        // check for pattern 3
        if (up_right == pattern3[0] && up_left == pattern3[1] && down_right == pattern3[2] && down_left == pattern3[3]) {
            totalCount++;
            // printf("Pattern 3 found\n");
            // location of the pattern
            // printf("Pattern location: %d, %d\n", xi, yi);
        }

        // check for pattern 4
        if (up_right == pattern4[0] && up_left == pattern4[1] && down_right == pattern4[2] && down_left == pattern4[3]) {
            totalCount++;
            // printf("Pattern 4 found\n");
            // location of the pattern
            // printf("Pattern location: %d, %d\n", xi, yi);
        }

    }

    free(x);
    free(y);

    return totalCount;
}



int main() {
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    char *filename = "input.txt";
    Crossword crossword = ReadFile(filename);

    int count = FindNumberofXMAS(crossword);
    printf("Number of 'XMAS' found: %d\n", count);

    count = FindNumberofPattern(crossword);
    printf("Number of X-MAS patterns found: %d\n", count);

    EndTimer(&start, &end);
    printf("End of test\n");

    return 0;
}
