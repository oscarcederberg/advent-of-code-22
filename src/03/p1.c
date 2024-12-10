#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHARS 64

int main() {
    int i, j;
    int skip = 0;
    int half_point;
    int character;
    int priority;
    int num_duplicates = 0;
    int *duplicates = (int *) calloc(num_duplicates, sizeof(int));
    char line_buffer[MAX_CHARS];

    while(fgets(line_buffer, MAX_CHARS, stdin) != NULL) {
        line_buffer[strcspn(line_buffer, "\r\n")] = 0;
        half_point = strlen(line_buffer) / 2;

        for (i = 0, skip = 0; line_buffer[i] != 0 && !skip; i++) {
            character = line_buffer[i];

            if (character >= 'A' && character <= 'Z') {
                priority = character - 'A' + 27;
            } else if (character >= 'a' && character <= 'z') {
                priority = character - 'a' + 1;
            } else {
                continue;
            }

            line_buffer[i] = priority;

            if (i < half_point) {
                continue;
            }
            for (j = 0; j < half_point; j++) {
                if (priority != line_buffer[j]) {
                    continue;
                }
                num_duplicates++;
                duplicates = (int *) realloc(duplicates, num_duplicates * sizeof(int));
                duplicates[num_duplicates - 1] = priority;
                skip = 1;
                break;
            }
        }
    }

    int sum_duplicates = 0;
    for (i = 0; i < num_duplicates; i++) {
        sum_duplicates += duplicates[i];
    }

    printf("the sum of duplicated priorities are %d\n", sum_duplicates);

    free(duplicates);
}