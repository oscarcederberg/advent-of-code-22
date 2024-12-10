#include <stdio.h>
#include <stdlib.h>

#define MAX_CHARS 64
#define MAX_DIGITS_FORMAT "%64d"

int overlaps_range(int first_from, int first_to, int second_from, int second_to) {
    return (second_from <= first_to && second_to >= first_to) ||
        (first_from <= second_to && first_to >= second_to);
}

int main() {
    int first_from, first_to, second_from, second_to;
    int weird_pairs = 0;

    while (scanf(MAX_DIGITS_FORMAT"-"MAX_DIGITS_FORMAT","MAX_DIGITS_FORMAT"-"MAX_DIGITS_FORMAT,
            &first_from, &first_to, &second_from, &second_to) == 4) {
        if (overlaps_range(first_from, first_to, second_from, second_to)) {
            weird_pairs++;
        }
    }

    printf("there are %d weird pairs\n", weird_pairs);
}