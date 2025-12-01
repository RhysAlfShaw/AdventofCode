#include <stdio.h>

int main() {
    int current_floor = 0;
    int c; // 'int' is required to handle EOF correctly, not 'char'

    // getchar() reads from the standard input (console or piped file)
    while ((c = getchar()) != EOF) {
        if (c == '(') {
            current_floor++;
        } 
        else if (c == ')') {
            current_floor--;
        }
    }

    printf("Santa ends up on floor: %d\n", current_floor);
    return 0;
}