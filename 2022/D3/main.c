#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void AdventCode2024() {
    printf("Advent Code 2024 Test!\n");
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
    char **items;  // Changed to array of strings
    char **shared; // the shared items between the two rucksacks
    int capacity;
} Rucksacks;

void initRucksacks(Rucksacks *arr, int capacity) {
    arr->capacity = capacity;
    // Allocate array of string pointers
    arr->items = malloc(capacity * sizeof(char*));
    arr->shared = malloc(capacity * sizeof(char*));
    if (arr->items == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    
    // Allocate memory for each string
    for (int i = 0; i < capacity; i++) {
        arr->items[i] = malloc(54 * sizeof(char));
        if (arr->items[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
}

void freeRucksacks(Rucksacks *arr) {
    if (arr->items != NULL) {
        for (int i = 0; i < arr->capacity; i++) {
            free(arr->items[i]);
        }
        free(arr->items);
    }
}

int length_of_file(const char *input_file) {
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file: %s\n", input_file);
        exit(1);
    }
    char line[1024];
    int items = 0;
    while (fgets(line, sizeof(line), file)) {
        items++;
    }
    fclose(file);
    return items;
}

void createSharedRucksack(Rucksacks *rucksacks) {
    for (int i = 0; i < rucksacks->capacity; i++) {

        int lenght = strlen(rucksacks->items[i]);
        int mid = lenght / 2;
        
        char *first = malloc(mid * sizeof(char));
        char *second = malloc(mid * sizeof(char));
    
        printf("Lenght: %d\n", lenght);
        printf("Mid: %d\n", mid);

        strncpy(first, rucksacks->items[i], mid);
        first[mid] = '\0';
        strncpy(second, rucksacks->items[i] + mid, lenght - mid);
        second[lenght - mid] = '\0';

        printf("First: %s\n", first);
        printf("Second: %s\n", second);


    }
}

Rucksacks readInputData(const char *input_file) {
    int items = length_of_file(input_file);
    
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file: %s\n", input_file);
        exit(1);
    }
    
    printf("Number of items: %d\n", items);
    Rucksacks rucksacks;
    initRucksacks(&rucksacks, items);
    
    char line[54];
    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        // remove newline character
        line[strcspn(line, "\n")] = 0;
        strcpy(rucksacks.items[i], line);
        i++;
    }

    createSharedRucksack(&rucksacks);

    fclose(file);
    return rucksacks;
}





int main() {
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);
    
    char input_file[] = "test_input.txt";
    
    Rucksacks rucksackslist = readInputData(input_file);
    
    // print content of rucksacks
    for (int i = 0; i < rucksackslist.capacity; i++) {
        printf("Item %d: %s\n", i, rucksackslist.items[i]);
    }
    
    // Code for the challenge goes here
    
    EndTimer(&start, &end);
    printf("End of test\n");
    
    // Clean up
    freeRucksacks(&rucksackslist);
    
    return 0;
}