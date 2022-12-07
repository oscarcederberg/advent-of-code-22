#include <stdio.h>

void update_total(unsigned int *total, unsigned int opponent_choice, unsigned int player_choice) {
    if (opponent_choice == player_choice) {
        *total += (player_choice + 1) + 3;
    } else if ((player_choice + 1) % 3 == opponent_choice) {
        *total += (player_choice + 1);
    } else {
        *total += (player_choice + 1) + 6;
    }
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