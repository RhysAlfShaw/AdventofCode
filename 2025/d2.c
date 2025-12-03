#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef unsigned long long ull;

#define K_MAX 6 
#define MAX_INVALID_IDS 1000000

ull invalid_ids[MAX_INVALID_IDS];
int invalid_count = 0;

ull power_of_10(int k) {
    ull p = 1;
    for (int i = 0; i < k; i++) {
        if (p > ULLONG_MAX / 10) return ULLONG_MAX;
        p *= 10;
    }
    return p;
}
int compare_ull(const void *a, const void *b) {
    ull arg1 = *(const ull*)a;
    ull arg2 = *(const ull*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

void generate_invalid_ids_part1() {
    invalid_count = 0;
    
    for (int k = 1; k <= K_MAX; k++) {
        ull p_k = power_of_10(k);
        if (p_k == ULLONG_MAX) break; 
        ull base_multiplier = p_k + 1;
        
        ull start_x = (k == 1) ? 1 : power_of_10(k - 1);
        ull end_x = p_k - 1;

        for (ull x = start_x; x <= end_x; x++) {
            if (base_multiplier > ULLONG_MAX / x) break; 

            ull N = x * base_multiplier;

            if (invalid_count < MAX_INVALID_IDS) {
                invalid_ids[invalid_count++] = N;
            } else {
                goto generation_end;
            }
        }
    }
generation_end:
    qsort(invalid_ids, invalid_count, sizeof(ull), compare_ull);
}

void generate_invalid_ids_part2() {
    invalid_count = 0;
    const int L_MAX = 12; 

    for (int k = 1; k <= L_MAX / 2; k++) { 
        ull p_k_minus_1 = power_of_10(k - 1); 
        ull p_k = power_of_10(k); 
        
        ull start_x = (k == 1) ? 1 : p_k_minus_1;
        ull end_x = p_k - 1;

        for (int R = 2; R * k <= L_MAX; R++) {
            
            ull multiplier = 0;
            ull term = 1;
            int has_overflow = 0;
            
            for (int i = 0; i < R; i++) {
                if (multiplier > ULLONG_MAX - term) { has_overflow = 1; break; }
                multiplier += term;
                
                if (i < R - 1) {
                    ull factor = p_k;
                    if (term > ULLONG_MAX / factor) { has_overflow = 1; break; }
                    term *= factor;
                }
            }

            if (has_overflow) continue; 

            for (ull x = start_x; x <= end_x; x++) {
                if (multiplier > ULLONG_MAX / x) break;
                ull N = x * multiplier;
                
                if (invalid_count < MAX_INVALID_IDS) {
                    invalid_ids[invalid_count++] = N;
                } else {
                    goto generation_end;
                }
            }
        }
    }
    
generation_end:    
    qsort(invalid_ids, invalid_count, sizeof(ull), compare_ull);

    if (invalid_count > 0) {
        int write_idx = 1;
        for (int read_idx = 1; read_idx < invalid_count; read_idx++) {
            if (invalid_ids[read_idx] != invalid_ids[read_idx - 1]) {
                invalid_ids[write_idx++] = invalid_ids[read_idx];
            }
        }
        invalid_count = write_idx;
    }
}

ull solve(const char *input) {
    
    ull total_sum = 0;
    
    char *input_copy = strdup(input);
    if (!input_copy) {
        perror("Memory allocation failed");
        return 0;
    }

    char *token = strtok(input_copy, ",");

    while (token != NULL) {
        char *dash = strchr(token, '-');
        
        if (dash) {
            *dash = '\0';
            const char *start_str = token;
            const char *end_str = dash + 1;

            ull A = strtoull(start_str, NULL, 10);
            ull B = strtoull(end_str, NULL, 10);
            
            int start_index = 0;
            while (start_index < invalid_count && invalid_ids[start_index] < A) {
                start_index++;
            }
            
            for (int i = start_index; i < invalid_count; i++) {
                ull N = invalid_ids[i];
                
                if (N > B) {
                    break; 
                }
                
                total_sum += N;
            }
        }
        
        token = strtok(NULL, ",");
    }

    free(input_copy);
    return total_sum;
}

const char *EXAMPLE_INPUT = 
    "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,"
    "1698522-1698528,446443-446449,38593856-38593862,565653-565659,"
    "824824821-824824827,2121212118-2121212124";

const char *INPUT = 
    "269194394-269335492,62371645-62509655,958929250-958994165,"
    "1336-3155,723925-849457,4416182-4470506,1775759815-1775887457,"
    "44422705-44477011,7612653647-7612728309,235784-396818,751-1236,"
    "20-36,4-14,9971242-10046246,8796089-8943190,34266-99164,"
    "2931385381-2931511480,277-640,894249-1083306,648255-713763,"
    "19167863-19202443,62-92,534463-598755,93-196,2276873-2559254,"
    "123712-212673,31261442-31408224,421375-503954,8383763979-8383947043,"
    "17194-32288,941928989-941964298,3416-9716";


int main() {
    printf("\n===================================\n");
    printf("     Advent of Code Day 2 Results  \n");
    printf("===================================\n");
    generate_invalid_ids_part1();
    ull result_part1 = solve(INPUT);
    printf("--- Part 1 ---\n");
    printf("Result:   %llu\n\n", result_part1);
    
    generate_invalid_ids_part2();
    ull result_part2 = solve(INPUT);
    printf("--- Part 2 ---\n");
    printf("Result:   %llu\n", result_part2);
        
    return 0;
}