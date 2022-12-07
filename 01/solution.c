#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_CHARS 64

int main () {    
    unsigned int elfs = 1;
    unsigned int elf_with_most_calories = 1;
    unsigned int *calories_per_elf = (unsigned int *) calloc(elfs, sizeof(unsigned int));
    char line_buffer[MAX_CHARS];

    while(fgets(line_buffer, MAX_CHARS, stdin) != NULL) {
        if(strcmp(line_buffer, "\n") == 0 || strcmp(line_buffer,"\r\n") == 0) {
            elfs++;
            calories_per_elf = (unsigned int *) realloc(calories_per_elf, elfs * sizeof(unsigned int));
            calories_per_elf[elfs - 1] = 0;
            continue;
        }

        calories_per_elf[elfs - 1] += atol(line_buffer);
        if (calories_per_elf[elfs - 1] > calories_per_elf[elf_with_most_calories - 1]) {
            elf_with_most_calories = elfs;
        }
    }
    
    printf("there were %u elfs on the expedition\nelf %u carried the most calories\nthey carried %u calories\n", elfs, elf_with_most_calories, calories_per_elf[elf_with_most_calories - 1]);
}