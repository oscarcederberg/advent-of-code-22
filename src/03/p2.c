#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_CHARS 64
#define GROUP_SIZE 3

int is_in(int item, char *buffer) {
    for (size_t i = 0; buffer[i] != 0; i++) {
        if (buffer[i] == item)
            return 1;
    }
    return 0;
}

int main() {
    int i, j, k;
    int skip = 0;
    int num_badges = 0;
    int *badges = (int *) calloc(num_badges, sizeof(int));
    char line_buffers[GROUP_SIZE][MAX_CHARS];

    while(!skip) {
        for (i = 0; i < GROUP_SIZE; i++){
            if (fgets(line_buffers[i], MAX_CHARS, stdin) == NULL) {
                skip = 1;
                break;
            }
            line_buffers[i][strcspn(line_buffers[i], "\r\n")] = 0;
        }

        if (skip)
            break;

        num_badges++;
        badges = (int *) realloc(badges, num_badges * sizeof(int));

        for (i = 0; i < line_buffers[0][i] != 0; i++) {
            char character = line_buffers[0][i];
            int priority = 0;

            if (!is_in(character, line_buffers[1]) ||
                !is_in(character, line_buffers[2]))
                continue;

            if (character >= 'A' && character <= 'Z') {
                priority = character - 'A' + 27;
            } else {
                priority = character - 'a' + 1;
            }
            badges[num_badges - 1] = priority;
            break;
        }
    }

    int sum_badges = 0;
    for (i = 0; i < num_badges; i++) {
        sum_badges += badges[i];
    }

    printf("the sum of badges' priorities are %d\n", sum_badges);

    free(badges);
}