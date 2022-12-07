#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHARS 64
#define TRACKED_ELVES 3

void update_elves_with_most_calories(unsigned int elf, unsigned int *calories_per_elf, unsigned int* elves_with_most_calories) {
    unsigned int elf_index = elf - 1;
    unsigned int temp;

    for (size_t i = 0; i < TRACKED_ELVES; i++) {
        if (calories_per_elf[elf_index] > calories_per_elf[elves_with_most_calories[i]]) {
            temp = elves_with_most_calories[i];
            elves_with_most_calories[i] = elf_index;
            elf_index = temp;
        }
    }
}

int main () {
    unsigned int num_elves = 1;
    unsigned int elves_with_most_calories[TRACKED_ELVES] = { 0 };
    unsigned int most_calories_sum = 0;
    unsigned int *calories_per_elf = (unsigned int *) calloc(num_elves, sizeof(unsigned int));
    char line_buffer[MAX_CHARS];

    while(fgets(line_buffer, MAX_CHARS, stdin) != NULL) {
        if(strcmp(line_buffer, "\n") == 0 || strcmp(line_buffer,"\r\n") == 0) {
            update_elves_with_most_calories(num_elves, calories_per_elf, elves_with_most_calories);
            num_elves++;
            calories_per_elf = (unsigned int *) realloc(calories_per_elf, num_elves * sizeof(unsigned int));
            calories_per_elf[num_elves - 1] = 0;
            continue;
        }

        calories_per_elf[num_elves - 1] += atol(line_buffer);
    }

    // NOTE: needs to update for the last elf
    update_elves_with_most_calories(num_elves, calories_per_elf, elves_with_most_calories);

    for (size_t i = 0; i < TRACKED_ELVES; i++) {
        most_calories_sum += calories_per_elf[elves_with_most_calories[i]];
    }

    printf("there were %u elves on the expedition\n", num_elves);
    for (size_t i = 0; i < TRACKED_ELVES; i++) {
        printf("elf %u carried %u calories\n", elves_with_most_calories[i] + 1, calories_per_elf[elves_with_most_calories[i]]);
    }
    printf("the top %u elves had a combined amount of %u calories\n", TRACKED_ELVES, most_calories_sum);

    free(calories_per_elf);
}