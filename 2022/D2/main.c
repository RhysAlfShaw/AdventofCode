#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

void AdventCode2024()
{
    printf("Advent Code 2024 Test! \n");
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
    int *opponent; // should be numbers 1 rock, 2 paper, 3 scissors
    int *player;   // should be numbers 1 rock, 2 paper, 3 scissors
    int *score;
    size_t capacity;
} StrategyArray;

void initStrategyArray(StrategyArray *arr, int capacity)
{
    arr->capacity = capacity; // Initial capacity based on length of the input
    arr->opponent = malloc(arr->capacity * sizeof(int));
    arr->player = malloc(arr->capacity * sizeof(int));
    arr->score = malloc(arr->capacity * sizeof(int));
    if (arr->opponent == NULL || arr->player == NULL || arr->score == NULL)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
}

int GetPlayerMove(char player, int opponent)
{
    // if player is X, then we need to lose

    if (player == 'X') // lose the game
    {
        if (opponent == 1) // oppenent is rock
        {
            return 3; // scissors
        }
        else if (opponent == 2) // opponent is paper
        {
            return 1; // rock
        }
        else if (opponent == 3) // opponent is scissors
        {
            return 2; // paper
        }
    }
    else if (player == 'Y') // draw the game
    {
        return opponent;
    }
    else if (player == 'Z') // win the game
    {
        if (opponent == 1) // oppenent is rock
        {
            return 2; // paper
        }
        else if (opponent == 2) // opponent is paper
        {
            return 3; // scissors
        }
        else if (opponent == 3) // opponent is scissors
        {
            return 1; // rock
        }
    }
    else
    {
        fprintf(stderr, "Invalid player move\n");
        exit(1);
    }
   
}

StrategyArray readDataToStrategyArray(const char *filename)
{
    // open the file

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Could not open file: %s\n", filename);
        exit(1);
    }

    int lenght_of_file = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file))
    {
        lenght_of_file++;
    }
    printf("Lenght of file: %d\n", lenght_of_file);

    StrategyArray arr;
    initStrategyArray(&arr, lenght_of_file);

    rewind(file); // go back to the beginning of the file
    int i = 0;
    while (fgets(line, sizeof(line), file))
    {
        // Remove newline character from the line
        line[strcspn(line, "\n")] = '\0';
        char *opponent, *player;
        // for each line in the file
        opponent = strtok(line, " "); // get the first token
        player = strtok(NULL, " ");   // get the second token

        if (strcmp(opponent, "A") == 0)
        {
            arr.opponent[i] = 1; // rock
        }
        else if (strcmp(opponent, "B") == 0)
        {
            arr.opponent[i] = 2; // paper
        }
        else if (strcmp(opponent, "C") == 0)
        {
            arr.opponent[i] = 3; // scissors
        }

        if (strcmp(player, "X") == 0) // lose
        {
            int play = GetPlayerMove('X', arr.opponent[i]);
            arr.player[i] = play;
        }
        else if (strcmp(player, "Y") == 0) // draw
        {
            int play = GetPlayerMove('Y', arr.opponent[i]);
            arr.player[i] = play;
        }
        else if (strcmp(player, "Z") == 0) // win
        {
            int play = GetPlayerMove('Z', arr.opponent[i]);
            arr.player[i] = play;
        }
        i++;
        printf("Opponent: %s, Player: %s\n", opponent, player);
    }

    fclose(file);
    // return arr;
    return arr;
}

void Calculate_Scores(StrategyArray strategyArray)
{
    // rules to make scores. (player + (6 if player wins))
    int win = 6;
    int draw = 3;
    int lose = 0;

    // win if 2 beats 1, 3 beats 2, 1 beats 3, draw if same, else lose, score is player score, if win add 6, if draw add both scores
   
    for (size_t i = 0; i < strategyArray.capacity; i++)
    {
        if (strategyArray.opponent[i] == 1 && strategyArray.player[i] == 2)
        {
            strategyArray.score[i] = strategyArray.player[i] + win;
        }
        else if (strategyArray.opponent[i] == 2 && strategyArray.player[i] == 3)
        {
            strategyArray.score[i] = strategyArray.player[i] + win;
        }
        else if (strategyArray.opponent[i] == 3 && strategyArray.player[i] == 1)
        {
            strategyArray.score[i] = strategyArray.player[i] + win;
        }
        else if (strategyArray.opponent[i] == strategyArray.player[i])
        {
            strategyArray.score[i] = strategyArray.player[i] + draw;
        }
        else
        {
            strategyArray.score[i] = strategyArray.player[i] + lose;
        }
    }
}

int Sum_Scores(StrategyArray strategyArray)
{
    int sum = 0;
    for (size_t i = 0; i < strategyArray.capacity; i++)
    {
        sum += strategyArray.score[i];
    }
    return sum;
}

int main()
{
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    char test_input_path[] = "input.txt";
    StrategyArray strategyArray = readDataToStrategyArray(test_input_path);

    Calculate_Scores(strategyArray);

    // print the numbers for debugging
    for (size_t i = 0; i < strategyArray.capacity; i++)
    {
        printf("%d %d %d\n", strategyArray.opponent[i], strategyArray.player[i], strategyArray.score[i]);
    }

    int sum = Sum_Scores(strategyArray);
    printf("Sum of scores: %d\n", sum);

    EndTimer(&start, &end);
    printf("End of test\n");
    return 0;
}
