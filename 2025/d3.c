#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define JOLTAGE_LENGTH_PART_TWO 12

int calculate_max_joltage_part_one(const char *line) {
    int max_joltage = 0;
    int len = strlen(line);

    if (len < 2) {
        return 0;
    }

    for (int i = 0; i < len - 1; i++) {
        int digit_A = line[i] - '0';

        for (int j = i + 1; j < len; j++) {
            int digit_B = line[j] - '0';
            
            int current_joltage = digit_A * 10 + digit_B;

            if (current_joltage > max_joltage) {
                max_joltage = current_joltage;
            }
        }
    }
    return max_joltage;
}

char* find_max_twelve_digit_joltage(const char *line) {
    int N = strlen(line);
    
    if (N < JOLTAGE_LENGTH_PART_TWO) {
        return NULL;
    }

    char *result = (char*)malloc(JOLTAGE_LENGTH_PART_TWO + 1);
    if (result == NULL) {
        perror("Memory allocation failed");
        return NULL;
    }
    result[JOLTAGE_LENGTH_PART_TWO] = '\0';

    int current_index = 0; 
    
    for (int k = 0; k < JOLTAGE_LENGTH_PART_TWO; k++) {
        int max_digit_val = -1;
        int best_index = -1;
        
        int required_remaining = JOLTAGE_LENGTH_PART_TWO - k;
        
        int search_end = N - required_remaining + 1;

        for (int i = current_index; i < search_end; i++) {
            int current_digit_val = line[i] - '0';
            
            if (current_digit_val > max_digit_val) {
                max_digit_val = current_digit_val;
                best_index = i;
            }
            
            if (max_digit_val == 9) {
                break;
            }
        }
        
        result[k] = line[best_index];
        current_index = best_index + 1;
    }

    return result;
}


void solve_challenge(FILE *file_ptr) {
    long total_output_joltage_p1 = 0;
    
    unsigned long long total_output_joltage_p2 = 0;

    char line[MAX_LINE_LENGTH];

    if (file_ptr == NULL) {
        fprintf(stderr, "Error: File pointer is NULL.\n");
        return;
    }
    
    while (fgets(line, sizeof(line), file_ptr) != NULL) {
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        if (len < 2) continue;

        int max_joltage_p1 = calculate_max_joltage_part_one(line);
        total_output_joltage_p1 += max_joltage_p1;
        
        if (len >= JOLTAGE_LENGTH_PART_TWO) {
            char *max_joltage_str_p2 = find_max_twelve_digit_joltage(line);
            
            if (max_joltage_str_p2 != NULL) {
                unsigned long long max_joltage_p2 = strtoull(max_joltage_str_p2, NULL, 10);
                total_output_joltage_p2 += max_joltage_p2;

                free(max_joltage_str_p2);
            }
        }
    }

    
    printf(" Part One (Max 2-Digit):\n");
    printf("   Total Output Joltage: **%ld**\n", total_output_joltage_p1);
    printf("-----------------------------------\n");
    printf(" Part Two (Max 12-Digit):\n");
    printf("   Total Output Joltage: **%llu**\n", total_output_joltage_p2);
    printf("===================================\n");
}

int main(void) {
    printf("\n===================================\n");
    printf("     Advent of Code Day 3 Results  \n");
    printf("===================================\n");
    const char *filename = "input3.txt";
    FILE *file = NULL;

    file = fopen(filename, "r");

    solve_challenge(file);
    
    fclose(file);

    return EXIT_SUCCESS;
}