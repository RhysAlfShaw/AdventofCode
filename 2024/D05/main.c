#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

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
    int page1;
    int page2;
} Rule;

bool ruleExists(Rule* rules, int rulesCount, int page1, int page2) {
    for (int i = 0; i < rulesCount; i++) {
        if (rules[i].page1 == page1 && rules[i].page2 == page2) {
            return true;
        }
    }
    return false;
}

bool isUpdateInOrder(int* pages, int pagesCount, Rule* rules, int rulesCount) {
    for (int i = 0; i < pagesCount; i++) {
        for (int j = i + 1; j < pagesCount; j++) {
            if (ruleExists(rules, rulesCount, pages[j], pages[i])) {
                return false;
            }
        }
    }
    return true;
}

int* correctUpdateOrder(int* pages, int pagesCount, Rule* rules, int rulesCount) {
    int* newOrder = malloc(pagesCount * sizeof(int));
    if (!newOrder) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    memcpy(newOrder, pages, pagesCount * sizeof(int));
    
    bool changed;
    do {
        changed = false;
        for (int i = 0; i < pagesCount - 1; i++) {
            for (int j = i + 1; j < pagesCount; j++) {
                if (ruleExists(rules, rulesCount, newOrder[j], newOrder[i])) {
                    // Swap elements
                    int temp = newOrder[i];
                    newOrder[i] = newOrder[j];
                    newOrder[j] = temp;
                    changed = true;
                    break;
                }
            }
            if (changed) break;
        }
    } while (changed);
    
    return newOrder;
}


#define MAX_RULES 10000
#define MAX_PAGES 1000
#define MAX_LINE_LENGTH 10000
#define MAX_UPDATES 10000


// Function to read rules from the file
int readRules(FILE* file, Rule* rules, int maxRules) {
    char line[MAX_LINE_LENGTH];
    int rulesCount = 0;

    while (fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // Skip empty lines
        if (strlen(line) == 0) continue;

        if (strchr(line, '|')) {
            int page1, page2;
            if (sscanf(line, "%d|%d", &page1, &page2) == 2) {
                if (rulesCount < maxRules) {
                    rules[rulesCount].page1 = page1;
                    rules[rulesCount].page2 = page2;
                    rulesCount++;
                } else {
                    fprintf(stderr, "Maximum rules limit reached\n");
                    break;
                }
            }
        } else {
            break;
        }
    }

    return rulesCount;
}

int* parsePagesFromLine(char* line, int* pagesCount) {
    *pagesCount = 0;
    static int pages[MAX_PAGES]; 

    char* token = strtok(line, ",");
    while (token && *pagesCount < MAX_PAGES) {
        pages[(*pagesCount)++] = atoi(token);
        token = strtok(NULL, ",");
    }

    return pages;
}

void processUpdates(FILE* file, Rule* rules, int rulesCount, 
                    int* partOneResult, int* partTwoResult, int* updateCount) {
    char line[MAX_LINE_LENGTH];
    *partOneResult = 0;
    *partTwoResult = 0;
    *updateCount = 0;

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        if (strlen(line) == 0) continue;

        int pagesCount = 0;
        int* pages = parsePagesFromLine(line, &pagesCount);

        if (pagesCount > 0) {
            if (isUpdateInOrder(pages, pagesCount, rules, rulesCount)) {
                *partOneResult += pages[(pagesCount - 1) / 2];
            } else {
                int* correctedOrder = correctUpdateOrder(pages, pagesCount, rules, rulesCount);
                if (correctedOrder) {
                    *partTwoResult += correctedOrder[(pagesCount - 1) / 2];
                    free(correctedOrder);
                }
            }
            (*updateCount)++;
        }
    }
}

int main() {
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    FILE* file = fopen("input.txt", "r");
    if (!file) {
        fprintf(stderr, "Error opening file: %s\n", strerror(errno));
        return 1;
    }

    Rule rules[MAX_RULES];
    int rulesCount = readRules(file, rules, MAX_RULES);

    int partOneResult, partTwoResult, updateCount;
    processUpdates(file, rules, rulesCount, 
                   &partOneResult, &partTwoResult, &updateCount);

    fclose(file);

    EndTimer(&start, &end);
    printf("Part 1 Result: %d\n", partOneResult);
    printf("Part 2 Result: %d\n", partTwoResult);
    printf("Total updates processed: %d\n", updateCount);
    printf("Done!\n");

    return 0;
}