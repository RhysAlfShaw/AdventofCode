#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void AdventCode2024() {
    printf("Advent Code 2024 Days! \n");
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


// struct for holding two lists of numbers
typedef struct {
    int *leftlist;
    int *rightlist;
    int *sortedleftlist;
    int *sortedrightlist;
    int *dist;
    int *similarity_score;
    int length;
} NumberLists;

NumberLists create_NumberList_from_file(const char *input_file){
    FILE *file = fopen(input_file, "r");
    

    NumberLists numberlists;
    numberlists.length = 0;

    char line[54];
    while (fgets(line, sizeof(line), file)) {
        numberlists.length++;
    }

    numberlists.leftlist = malloc(numberlists.length * sizeof(int));
    numberlists.rightlist = malloc(numberlists.length * sizeof(int));
    numberlists.sortedleftlist = malloc(numberlists.length * sizeof(int));
    numberlists.sortedrightlist = malloc(numberlists.length * sizeof(int));
    numberlists.dist = malloc(numberlists.length * sizeof(int));
    numberlists.similarity_score = malloc(numberlists.length * sizeof(int));

    rewind(file);

    int i = 0;
    while (fgets(line, sizeof(line), file)) {
        // remove newline character
        line[strcspn(line, "\n")] = 0;
        char *token = strtok(line, " ");
        numberlists.leftlist[i] = atoi(token);
        token = strtok(NULL, " ");
        numberlists.rightlist[i] = atoi(token);
        i++;
    }

    fclose(file);
    return numberlists;
}

// Custom comparator
int compare(const void* a, const void* b) {
      
    // If a is smaller, positive value will be returned
    return (*(int*)a - *(int*)b);
}

// function to sort the lists in ascending order
void create_sorted_lists(NumberLists *numberlists) {
    // create sorted list of both left and right lists sepertaely with no duplicates and using qsort
    for (int i = 0; i < numberlists->length; i++) {
        numberlists->sortedleftlist[i] = numberlists->leftlist[i];
        numberlists->sortedrightlist[i] = numberlists->rightlist[i];
    }
    qsort(numberlists->sortedleftlist, numberlists->length, sizeof(int), compare);
    qsort(numberlists->sortedrightlist, numberlists->length, sizeof(int), compare);

}

// calculate the distance between two points in between two lists and store in dist
void calculate_distance(NumberLists *numberlists) {
    for (int i = 0; i < numberlists->length; i++) {
        numberlists->dist[i] = abs(numberlists->sortedrightlist[i] - numberlists->sortedleftlist[i]);
    }
}

int sum_of_dist(NumberLists *numberlists) {
    int sum = 0;
    for (int i = 0; i < numberlists->length; i++) {
        sum += numberlists->dist[i];
    }
    return sum;
}

// calculate the similarty score 

void calculate_similarity_score(NumberLists *numberlists) {
    // calculate the similarity score as left number * (number of times it is in the right list)
    for (int i = 0; i < numberlists->length; i++) {
        int count = 0;
        for (int j = 0; j < numberlists->length; j++) {
            if (numberlists->leftlist[i] == numberlists->rightlist[j]) {
                count++;
            }
        }
        numberlists->similarity_score[i] = numberlists->leftlist[i] * count;
    }
}

int sum_of_similarity_score(NumberLists *numberlists) {
    int sum = 0;
    for (int i = 0; i < numberlists->length; i++) {
        sum += numberlists->similarity_score[i];
    }
    return sum;
}

int main() {

    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    char input_file[] = "input.txt";

    // Code for the challenge goes here
    NumberLists numberlists = create_NumberList_from_file(input_file);
    
    // // print content of numberlists
    // for (int i = 0; i < numberlists.length; i++) {
    //     printf("Left: %d, Right: %d\n", numberlists.leftlist[i], numberlists.rightlist[i]);
    // }

    create_sorted_lists(&numberlists);
    calculate_distance(&numberlists);
    calculate_similarity_score(&numberlists);
    
    // for (int i = 0; i < numberlists.length; i++) {
    //     printf("Sorted Left: %d, Sorted Right: %d, dist; %d, dist: %d \n", numberlists.sortedleftlist[i], numberlists.sortedrightlist[i], numberlists.dist[i], numberlists.similarity_score[i]);
    // }

    printf("Sum of dist: %d\n", sum_of_dist(&numberlists));

    printf("Sum of similarity score: %d\n", sum_of_similarity_score(&numberlists));

    EndTimer(&start, &end);

    printf("End of test\n");
    return 0;
}