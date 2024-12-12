#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

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
    int numberofreports;
    int **reports; // array of pointers to reports
    int *reportlevels;
    int *safe; // 0 if not safe, 1 if safe
} ReportArray;

ReportArray create_report_array_from_file(const char *file_path)
{

    ReportArray reportarray;

    FILE *file = fopen(file_path, "r");

    // get number of reports from the number of lines in the file
    int numberofreports = 0;

    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        numberofreports++;
    }

    // allocate memory for the reports and reportlevels
    reportarray.reports = (int **)malloc(numberofreports * sizeof(int *));
    reportarray.reportlevels = (int *)malloc(numberofreports * sizeof(int));
    reportarray.safe = (int *)malloc(numberofreports * sizeof(int));
    reportarray.numberofreports = numberofreports;

    // read the file again to get the reports and reportlevels
    rewind(file);
    int reportindex = 0;
    while (fgets(line, sizeof(line), file))
    {
        // line is 6 7 8 9 9.. save as array of integers 6, 7, 8, 9, 9..
        // printf("Line: %s\n", line);
        // for each character in the line add it to a list of integers
        int *report = (int *)malloc(10 * sizeof(int)); // max 10 integers in a report
        int reportlevel = 0;                           // number of integers in the report
        char *token = strtok(line, " ");               // split the line by space
        while (token != NULL)
        {                                      // for each token in the line
            report[reportlevel] = atoi(token); // convert the token to an integer
            // printf("Token: %s, Integer: %d\n", token, report[reportlevel]);
            reportlevel++;             // increment the number of integers in the report
            token = strtok(NULL, " "); // get the next token
        }

        reportarray.reports[reportindex] = report;           // add the report to the reportarray
        reportarray.reportlevels[reportindex] = reportlevel; // add the number of integers in the report
        reportindex++;
    }

    fclose(file);
    return reportarray;
}

int is_safe_gap(int *report, int reportlevel)
{
    // check if the gap between the levels is safe
    int safe = 1;
    for (int i = 0; i < reportlevel - 1; i++)
    {
        if (abs(report[i] - report[i + 1]) < 1 || abs(report[i] - report[i + 1]) > 3)
        {
            safe = 0;
            break;
        }
    }
    return safe;
}

int is_safe_inc_dec(int *report, int reportlevel)
{
    // create a list of increase and decrease 0 or 1. 1 for increase and 0 for decrease
    int *inc_dec = (int *)malloc(reportlevel * sizeof(int));
    // lenght if inc_dec
    for (int i = 0; i < reportlevel; i++)
    {
        // for each level check if the next level is greater or less than the current level
        if (i == 0)
        {
            if (report[i] < report[i + 1])
            {
                inc_dec[i] = 1;
            }
            else if (report[i] > report[i + 1])
            {
                inc_dec[i] = 0; // decrease
            }
        }
        else if (i < reportlevel - 1)
        {
            // check the one before and after
            if (report[i] < report[i + 1] && report[i] > report[i - 1])
            {
                inc_dec[i] = 1; // increase
            }
            else if (report[i] > report[i + 1] && report[i] < report[i - 1])
            {
                inc_dec[i] = 0; // decrease
            }
            else if (report[i] < report[i + 1] && report[i] < report[i - 1])
            {
                inc_dec[i] = 1; // increase
            }
            else if (report[i] > report[i + 1] && report[i] > report[i - 1])
            {
                inc_dec[i] = 0; // decrease
            }
            else
            {
                inc_dec[i] = -1; // no change
            }
        }
        else
        {
            if (report[i] > report[i - 1])
            {
                inc_dec[i] = 1; // decrease
            }
            else if (report[i] < report[i - 1])
            {
                inc_dec[i] = 0; // increase
            }
        }
    }

    // printf("Inc Dec: ");
    // for (int i = 0; i < reportlevel; i++)
    // {
    //     printf("%d ", inc_dec[i]);
    // }

    int safe = 1;

    int sum = 0;
    for (int i = 0; i < reportlevel; i++)
    {
        sum += inc_dec[i];
    }
    if (sum == 0 || sum == reportlevel)
    {
        safe = 1;
    }
    else
    {
        safe = 0;
    }

    return safe;
}

void calculate_safety(ReportArray reportarray)
{
    // calculate the safety of each report. the increment change between levels must be atleast 1 and max 3
    for (int i = 0; i < reportarray.numberofreports; i++)
    {

        int reportlevel = reportarray.reportlevels[i];
        int *report = reportarray.reports[i];

        int safe_gap = is_safe_gap(report, reportlevel);         
        int safe_inc_dec = is_safe_inc_dec(report, reportlevel); 

        reportarray.safe[i] = safe_gap && safe_inc_dec;
        
    }

    printf("Number of reports: %d\n", reportarray.numberofreports);
    //  section for part 2. if a report is unsafe, check if removing one level makes it safe.
    for (int j = 0; j < reportarray.numberofreports; j++)
    {  
        if (reportarray.safe[j] == 0) // if it's unsafe 
        {
            int *report = reportarray.reports[j];
            int reportlevel = reportarray.reportlevels[j];
            for (int i = 0; i < reportlevel; i++)
            {
                int *newreport = (int *)malloc((reportlevel - 1) * sizeof(int));
                int newreportlevel = reportlevel - 1;
                int k = 0;
                for (int l = 0; l < reportlevel; l++)
                {
                    if (l != i)
                    {
                        newreport[k] = report[l];
                        k++;
                    }
                }
                
                int safe_gap = is_safe_gap(newreport, newreportlevel);
                int safe_inc_dec = is_safe_inc_dec(newreport, newreportlevel);
                if (safe_gap && safe_inc_dec)
                {
                    reportarray.safe[j] = 1;
                    break;
                }
                free(newreport);
            }
        }
    }

}

int sum_of_safe_reports(ReportArray reportarray)
{
    int sum = 0;
    for (int i = 0; i < reportarray.numberofreports; i++)
    {
        if (reportarray.safe[i])
        {
            sum += 1;
        }
    }
    return sum;
}

int main()
{
    AdventCode2024();
    struct timespec start, end;
    StartTimer(&start);

    // Code for the challenge goes here
    char input_file[] = "input.txt";
    ReportArray reportarray = create_report_array_from_file(input_file);
    calculate_safety(reportarray);
    // print content of reportarray
    for (int i = 0; i < reportarray.numberofreports; i++)
    {
        printf("Report %d: ", i);
        for (int j = 0; j < reportarray.reportlevels[i]; j++)
        {
            printf("%d ", reportarray.reports[i][j]);
        }
        printf("Level: %d", reportarray.reportlevels[i]);
        printf(" Safe: %d", reportarray.safe[i]);
        printf("\n");
    }
    printf("Number of reports: %d\n", reportarray.numberofreports);
    printf("Number of safe reports: %d\n", sum_of_safe_reports(reportarray));
    EndTimer(&start, &end);
    printf("End of test\n");
    return 0;
}