#include <stdio.h>
#include <stdlib.h>

void update_total(unsigned int *total, unsigned int opponent_choice, unsigned int outcome) {
    *total += ((opponent_choice + outcome + 2) % 3 + 1) + (3 * outcome);
}

int main() {
    int character;
    unsigned int opponent_choice = 0;
    unsigned int total_points = 0;

    do {
        character = getchar();
        switch (character) {
        case 'A':
        case 'B':
        case 'C':
            opponent_choice = character - 'A';
            break;

        case 'X':
        case 'Y':
        case 'Z':
            update_total(&total_points, opponent_choice, character - 'X');
            break;

        default:
            break;
        }
    } while (character != EOF);

    printf("total score is %u\n", total_points);
}