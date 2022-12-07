#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHARS 64

int main () {
    unsigned int num_elves = 1;
    unsigned int elf_with_most_calories = 1;
    unsigned int *calories_per_elf = (unsigned int *) calloc(num_elves, sizeof(unsigned int));
    char line_buffer[MAX_CHARS];

    while(fgets(line_buffer, MAX_CHARS, stdin) != NULL) {
        if(strcmp(line_buffer, "\n") == 0 || strcmp(line_buffer,"\r\n") == 0) {
            num_elves++;
            calories_per_elf = (unsigned int *) realloc(calories_per_elf, num_elves * sizeof(unsigned int));
            calories_per_elf[num_elves - 1] = 0;
            continue;
        }

        calories_per_elf[num_elves - 1] += atol(line_buffer);
        if (calories_per_elf[num_elves - 1] > calories_per_elf[elf_with_most_calories - 1]) {
            elf_with_most_calories = num_elves;
        }
    }

    printf("there were %u elfs on the expedition\nelf %u carried the most calories\nthey carried %u calories\n", num_elves, elf_with_most_calories, calories_per_elf[elf_with_most_calories - 1]);
}