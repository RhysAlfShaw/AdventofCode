#include <stdio.h>
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

int main() {
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    // Code for the challenge goes here

    EndTimer(&start, &end);
    printf("End of test\n");
    return 0;
}