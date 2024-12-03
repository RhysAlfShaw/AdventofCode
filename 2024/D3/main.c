#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void AdventCode2024()
{
    printf("Advent Code 2024 Day 3! \n");
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

typedef struct
{
    char *content;
    int contentLength;
} CuruptedMemory;


CuruptedMemory ReadFile(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        exit(1);
    }

    CuruptedMemory memory;
    memory.content = NULL;
    memory.contentLength = 0;
    char buffer[1024]; // Temporary buffer to read file in chunks

    while (fgets(buffer, sizeof(buffer), file))
    {
        int bufferLength = strlen(buffer);
        memory.content = realloc(memory.content, memory.contentLength + bufferLength + 1);
        strcpy(memory.content + memory.contentLength, buffer);
        memory.contentLength += bufferLength;
    }
    fclose(file);
    return memory;
}



typedef struct
{
    char **Kernals;
    int KernalsCount;
} Kernals;


Kernals generate_valid_Kernals()
{
    Kernals kernals;
    kernals.KernalsCount = 1000 * 1000;
    kernals.Kernals = malloc(kernals.KernalsCount * sizeof(char *));
    int kernalCount = 0;
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            kernals.Kernals[kernalCount] = malloc(256);
            sprintf(kernals.Kernals[kernalCount], "mul(%d,%d)", i, j);
            kernalCount++;
        }
    }
    printf("Kernal Count: %d\n", kernalCount);
    // for (int i = 0; i < 10; i++)
    // {
    //     printf("Kernal: %s\n", kernals.Kernals[i]);
    // }
    return kernals;
}


typedef struct
{
    char **Kernal;
    int kernalCount;
} DoDontKernal;


typedef struct
{
    int *mask;
} DoDontMask;


void init_DoDontMask(DoDontMask *mask, CuruptedMemory memory)
{
    mask->mask = malloc(memory.contentLength * sizeof(int));
    // Initialize mask with 0 (valid)
    for (int i = 0; i < memory.contentLength; i++)
    {
        mask->mask[i] = 0; // Default to valid
    }
}


DoDontKernal generate_do_dont_Kernal()
{
    DoDontKernal kernals;
    kernals.kernalCount = 2;
    kernals.Kernal = malloc(kernals.kernalCount * sizeof(char *));
    kernals.Kernal[0] = "do()";
    kernals.Kernal[1] = "don't()";
    return kernals;
}


void process_dodont_mask(DoDontMask *mask, CuruptedMemory memory, DoDontKernal dodont)
{
    int globalMaskState = 0; 

    for (int i = 0; i < memory.contentLength; i++)
    {
        for (int j = 0; j < dodont.kernalCount; j++)
        {
            if (strncmp(memory.content + i, dodont.Kernal[j], strlen(dodont.Kernal[j])) == 0)
            {
                if (strcmp(dodont.Kernal[j], "do()") == 0)
                {
                    for (int k = i; k < memory.contentLength; k++)
                    {
                        mask->mask[k] = 0; 
                    }
                    globalMaskState = 0; 
                }
                else if (strcmp(dodont.Kernal[j], "don't()") == 0)
                {
                    for (int k = i; k < memory.contentLength; k++)
                    {
                        mask->mask[k] = 1; 
                    }
                    globalMaskState = 1;
                }
            }
        }
    }
}

typedef struct
{
    int *kernelPosition;
    int kernalCount;
    char **FoundKernals;
} KernalResult;


KernalResult ValidMulLocation(CuruptedMemory memory)
{
    Kernals kernals = generate_valid_Kernals();
    DoDontKernal dodont = generate_do_dont_Kernal();

    int *kernelPosition = malloc(memory.contentLength * sizeof(int)); 
    int kernalCount = 0;
    char **validKernals = malloc(memory.contentLength * sizeof(char *)); 

    for (int j = 0; j < kernals.KernalsCount; j++)
    {
        char *found = strstr(memory.content, kernals.Kernals[j]);
        if (found != NULL)
        {
            int kernalPosition = found - memory.content;
            kernelPosition[kernalCount] = kernalPosition;
            validKernals[kernalCount] = strdup(kernals.Kernals[j]);
            kernalCount++;
        }
     }
    
    // create a mask for the dodont kernals
    DoDontMask mask;
    init_DoDontMask(&mask, memory);
    process_dodont_mask(&mask, memory, dodont);

    // for (int i = 0; i < memory.contentLength; i++)
    // {
    //     printf("%d", mask.mask[i]);
    // }
    // printf("\n");

    // remove the invalid kernals from the mask
    int newKernalCount = 0;
    for (int i = 0; i < kernalCount; i++)
    {
        if (mask.mask[kernelPosition[i]] == 0)
        {
            newKernalCount++;
        }
    }

    char **compacted_FoundKernals = malloc(newKernalCount * sizeof(char *));
    int new_index = 0;


    for (int i = 0; i < kernalCount; i++)
    {
        if (mask.mask[kernelPosition[i]] == 0)
        {
            compacted_FoundKernals[new_index++] = validKernals[i];
        }
    }

    free(validKernals);


    validKernals = compacted_FoundKernals;
    kernalCount = newKernalCount; 
    KernalResult result;
    result.kernelPosition = kernelPosition;
    result.kernalCount = kernalCount;
    result.FoundKernals = validKernals;
    return result;
}

int CalculateKernals(KernalResult kernals)
{
    int result = 0;
    for (int i = 0; i < kernals.kernalCount; i++)
    {
        // find the mul() pattern
        if (strstr(kernals.FoundKernals[i], "mul") != NULL)
        {
            // find the numbers in the pattern
            char *token = strtok(kernals.FoundKernals[i], "(");
            token = strtok(NULL, ",");
            int num1 = atoi(token);
            // printf("Num1: %d\n", num1);
            token = strtok(NULL, ")");
            int num2 = atoi(token);
            // printf("Num2: %d\n", num2);  
            result += num1 * num2;
            // printf("Result: %d\n", result);
        }
    }
    return result;
}


int main()
{
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    char *input = "input.txt";

    CuruptedMemory memory = ReadFile(input);

    printf("\nContent Length: %d\n", memory.contentLength);

    KernalResult validKernals = ValidMulLocation(memory);
    
    // print the kernals
    // for (int i = 0; i < validKernals.kernalCount; i++)
    // {
    //     printf("Kernal: %s\n", validKernals.FoundKernals[i]);
    // }

    int summ = CalculateKernals(validKernals);

    printf("Sum of the kernals: %d\n", summ);

    EndTimer(&start, &end);
    printf("End of test\n");

    // Clean up allocated memory
    free(memory.content);
    free(validKernals.kernelPosition);
    return 0;
}
