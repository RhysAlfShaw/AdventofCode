#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *file = fopen("input1.txt", "r");

    int current_pos = 50; 
    const int DIAL_SIZE = 100;

    long long part1_password = 0;
    long long part2_password = 0;

    char direction;
    int distance;

    while (fscanf(file, " %c%d", &direction, &distance) == 2) {
        // is the rotations large enought for it too loop around?
        int full_rotations = distance / DIAL_SIZE;
        part2_password += full_rotations;

        int remainder = distance % DIAL_SIZE;

        for (int i = 0; i < remainder; i++) {
            if (direction == 'R') {
                current_pos++;
                if (current_pos >= DIAL_SIZE) {
                    current_pos = 0;
                }
            } else if (direction == 'L') {
                current_pos--;
                if (current_pos < 0) {
                    current_pos = DIAL_SIZE - 1;
                }
            }
            
            if (current_pos == 0) {
                part2_password++;
            }
        }
        if (current_pos == 0) {
            part1_password++;
        }
    }
    fclose(file);
    printf("Part 1 Password: %lld\n", part1_password);
    printf("Part 2 Password: %lld\n", part2_password);
    return 0;
}