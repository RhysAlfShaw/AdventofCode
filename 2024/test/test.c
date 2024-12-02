#include <stdio.h>
#include <string.h>

int main() {
    // Example strings
    char str1[100] = "Hello";
    char str2[100] = "World";
    char str3[100];

    // 1. Get the length of a string
    size_t len = strlen(str1);
    printf("Length of str1: %zu\n", len);  // Output: 5

    // 2. Copy str1 to str3
    strcpy(str3, str1);
    printf("str3 after strcpy: %s\n", str3);  // Output: Hello

    // 3. Concatenate str1 and str2
    strcat(str1, " ");
    strcat(str1, str2);
    printf("str1 after strcat: %s\n", str1);  // Output: Hello World

    // 4. Compare two strings
    int cmp_result = strcmp(str1, str3);
    if (cmp_result == 0) {
        printf("str1 and str3 are equal.\n");
    } else if (cmp_result < 0) {
        printf("str1 is less than str3.\n");
    } else {
        printf("str1 is greater than str3.\n");
    }

    // 5. Find a substring
    char *substr = strstr(str1, "World");
    if (substr) {
        printf("Substring found: %s\n", substr);  // Output: World
    } else {
        printf("Substring not found.\n");
    }

    return 0;
}
