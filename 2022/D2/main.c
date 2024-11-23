#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

void AdventCode2024()
{
    printf("Advent Code 2022 Challenge 1 Calorie Counting! \n");
}

void StartTimer(struct timespec *start)
{
    clock_gettime(CLOCK_MONOTONIC, start);
}

void EndTimer(struct timespec *start, struct timespec *end)
{

    clock_gettime(CLOCK_MONOTONIC, end);
    double seconds = (end->tv_sec - start->tv_sec) +
                     (end->tv_nsec - start->tv_nsec) / 1000000000.0;
    printf("Time for execution: %f seconds\n", seconds);
}

// structure to hold our dynamic array of integers
typedef struct
{
    int *numbers;
    size_t size;
    size_t capacity;
} NumberArray;

// Initialize the array structure
void initNumberArray(NumberArray *arr)
{
    arr->capacity = 10; // Initial capacity
    arr->size = 0;
    arr->numbers = malloc(arr->capacity * sizeof(int));
    if (arr->numbers == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
}

// Resize array if needed
void ensureCapacity(NumberArray *arr)
{
    if (arr->size >= arr->capacity)
    {
        arr->capacity *= 2;
        int *temp = realloc(arr->numbers, arr->capacity * sizeof(int));
        if (temp == NULL)
        {
            fprintf(stderr, "Failed to reallocate memory\n");
            free(arr->numbers);
            exit(1);
        }
        arr->numbers = temp;
    }
}

// Function to free the dynamically allocated memory
void freeNumberArray(NumberArray *arr)
{
    free(arr->numbers);
}

// Helper function to check if a line is empty or contains only spaces
int isEmptyOrWhitespace(const char *line)
{
    while (*line)
    {
        if (!isspace((unsigned char)*line))
            return 0; // Line has something other than whitespace
        line++;
    }
    return 1; // Line is empty or only whitespace
}

// Read integers from file
NumberArray readNumbersFromFile(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", filename);
        exit(1);
    }

    NumberArray arr;
    initNumberArray(&arr);

    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        if (isEmptyOrWhitespace(line))
        {
            ensureCapacity(&arr);
            arr.numbers[arr.size++] = 0;
        }
        else
        {
            int number;
            if (sscanf(line, "%d", &number) == 1)
            {
                ensureCapacity(&arr);
                arr.numbers[arr.size++] = number;
            }
            else
            {
                ensureCapacity(&arr);
                arr.numbers[arr.size++] = 0;
            }
        }
    }
    fclose(file);
    return arr;
}

NumberArray sumNumbers(NumberArray numbers)
{
    // create a new array to hold the sum of the numbers between 0
    // does to account for the last number in the array

    NumberArray sumArray;
    initNumberArray(&sumArray);
    int sum = 0;
    for (size_t i = 0; i < numbers.size; i++)
    {
        if (numbers.numbers[i] == 0)
        {
            ensureCapacity(&sumArray);
            sumArray.numbers[sumArray.size++] = sum;
            sum = 0;
        }
        else
        {
            sum += numbers.numbers[i];
        }
    }
    // Account for the last sum if the array does not end with 0
    if (sum != 0 || (numbers.size > 0 && numbers.numbers[numbers.size - 1] == 0))
    {
        ensureCapacity(&sumArray);
        sumArray.numbers[sumArray.size++] = sum;
    }
    return sumArray;
}

void Index_of_Maximum(NumberArray sum_numbers)
{
    int max = sum_numbers.numbers[0];
    int index = 0;
    for (size_t i = 0; i < sum_numbers.size; i++)
    {
        if (sum_numbers.numbers[i] > max)
        {
            max = sum_numbers.numbers[i];
            index = i;
        }
    }
    printf("Index of maximum: %d\n", index + 1); // +1 because the index starts at 0

    printf("Value of maximum: %d\n", max);
    }


NumberArray SortNumberArray_by_decending_values(NumberArray sum_numbers)
{
    int temp;
    for (size_t i = 0; i < sum_numbers.size; i++)
    {
        for (size_t j = i + 1; j < sum_numbers.size; j++)
        {
            if (sum_numbers.numbers[i] < sum_numbers.numbers[j])
            {
                temp = sum_numbers.numbers[i];
                sum_numbers.numbers[i] = sum_numbers.numbers[j];
                sum_numbers.numbers[j] = temp;
            }
        }
    }
    return sum_numbers;
}

void Sum_of_Highest_Three(NumberArray sorted_sum_numbers)
{
    int sum = 0;
    for (size_t i = 0; i < 3; i++)
    {
        sum += sorted_sum_numbers.numbers[i];
        // printf("%d",sorted_sum_numbers.numbers[i]);
    }
    printf("Sum of the highest three numbers: %d\n", sum);
}

int main()
{
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    // create a string that holds the path
    char test_input_path[] = "input.txt";

    // read the numbers from the file
    NumberArray numbers = readNumbersFromFile(test_input_path);

    // // print the numbers for debugging
    // for (size_t i = 0; i < numbers.size; i++) 
    // {
    //     printf("%d\n", numbers.numbers[i]);
    // }

    
    NumberArray sum_numbers = sumNumbers(numbers);

    // // print the sum array for debugging
    // for (size_t i = 0; i < sum_numbers.size; i++)
    // {
    //     printf("Sum: %d\n", sum_numbers.numbers[i]);
    // }

    NumberArray sorted_sum_numbers = SortNumberArray_by_decending_values(sum_numbers);
    

    // for (size_t i = 0; i < sorted_sum_numbers.size; i++)
    // {
    //     printf("Sorted Sum: %d\n", sorted_sum_numbers.numbers[i]);
    // }

    Index_of_Maximum(sum_numbers); 

    // sum of the top highest three numbers

    Sum_of_Highest_Three(sorted_sum_numbers);

    printf("Test input path: %s\n", test_input_path);

    EndTimer(&start, &end);
    freeNumberArray(&numbers);
    printf("End of test\n");
    return 0;
}