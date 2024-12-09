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

char *ReadFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    // Start with a NULL content and initial content size of 0
    char *content = NULL;
    size_t content_size = 0;
    char buffer[10000];  // Increased buffer size to handle longer lines

    // Read the file line by line until EOF
    while (fgets(buffer, sizeof(buffer), file)) {
        size_t buffer_len = strlen(buffer);

        // Reallocate the content buffer only when necessary
        char *temp = realloc(content, content_size + buffer_len + 1);
        if (temp == NULL) {
            perror("Failed to allocate memory");
            exit(1);
        }

        content = temp;
        strcpy(content + content_size, buffer);
        content_size += buffer_len;
    }

    fclose(file);
    return content;
}

char *Convert_to_spaced_datablocks(char *content) {
    size_t len = strlen(content);
    char *spaced_content = malloc(len + 1);
    if (spaced_content == NULL) {
        perror("Failed to allocate memory");
        exit(1);
    }

    int CurrentID = 0;
    size_t current_len = 0;

    for (size_t i = 0; i < len; i++) {
        char value_char = content[i];
        int value = atoi(&value_char);

        if (i % 2 == 0) {  // even index
            char CurrentID_str[20];
            snprintf(CurrentID_str, sizeof(CurrentID_str), "%d", CurrentID);

            size_t new_len = current_len + value * strlen(CurrentID_str);
            char *temp = realloc(spaced_content, new_len + 1);
            if (temp == NULL) {
                perror("Failed to allocate memory");
                exit(1);
            }

            spaced_content = temp;
            for (int j = 0; j < value; j++) {
                strcpy(spaced_content + current_len + j * strlen(CurrentID_str), CurrentID_str);
            }
            current_len += value * strlen(CurrentID_str);
            CurrentID++;
        } else {  // odd index
            size_t new_len = current_len + value + 1;
            char *temp = realloc(spaced_content, new_len);
            if (temp == NULL) {
                perror("Failed to allocate memory");
                exit(1);
            }

            spaced_content = temp;
            for (int j = 0; j < value; j++) {
                spaced_content[current_len + j] = '.';
            }
            current_len += value;
        }
    }

    spaced_content[current_len] = '\0';  // Null-terminate the string
    return spaced_content;
}

char* Compactblocks(char* data) {
    size_t len = strlen(data);
    for (int i = 0; i < len / 2; i++) {
        char current = data[len - i - 1];
        for (int j = 0; j < len - i - 1; j++) {
            if (data[j] == '.' && j < len - i - 1) {
                // Swap the two characters
                char temp = data[len - i - 1];
                data[len - i - 1] = data[j];
                data[j] = temp;
                break;
            }
        }
    }

    return data;
}

long long Calculate_block_checksum(char* data) {
    long long checksum = 0;
    for (unsigned long i = 0; i < strlen(data); i++) {
        if (data[i] == '.') continue;

        if (data[i] >= '0' && data[i] <= '9') {
            checksum += i * (data[i] - '0');
        }
    }

    return checksum;
}

int main() {
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    // read in the file
    char *filename = "test_input.txt";
    char *content = ReadFile(filename);
    char *spaced_content = Convert_to_spaced_datablocks(content);
    char *sorted_blocks = Compactblocks(spaced_content);
    long long checksum = Calculate_block_checksum(sorted_blocks);

    // print the content
    printf("Content: %s\n", content);
    printf("Length: %ld\n", strlen(content));
    printf("Spaced Content: %s\n", spaced_content);
    printf("Sorted Blocks: %s\n", sorted_blocks);
    printf("Checksum: %lli\n", checksum);

    char *filename2 = "input.txt";
    char *content2 = ReadFile(filename2);
    char *spaced_content2 = Convert_to_spaced_datablocks(content2);
    char *sorted_blocks2 = Compactblocks(spaced_content2);
    long long checksum2 = Calculate_block_checksum(sorted_blocks2);

    // print the content
    printf("Checksum: %lli\n", checksum2);

    EndTimer(&start, &end);
    printf("End of test\n");

    return 0;
}
