#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_CHARS 516

enum value_type {
    ERR,
    LIST,
    VALUE
};

struct value_t {
    enum value_type type;
    int value;
    int count;
    struct value_t *values;
};

void parse_packets(struct packet_t *left, char *left_buffer, struct packet_t *right, char *right_buffer) {
}

int main(void) {
    size_t sum_of_right_ordered_indicies = 0;
    size_t pairs = 0;
    struct value_t left;
    struct value_t right;
    char left_buffer[MAX_CHARS] = {0, };
    char right_buffer[MAX_CHARS] = {0, };

    while (fgets(left_buffer, MAX_CHARS, stdin) != NULL &&
        fgets(right_buffer, MAX_CHARS, stdin) != NULL) {
        pairs++;
        printf("pair %lu:\n%s%s", pairs, left_buffer, right_buffer);

        left = (struct value_t) { .type = ERR, .value = 0, .count = 0, .values = NULL };
        right = (struct value_t) { .type = ERR, .value = 0, .count = 0, .values = NULL };
        parse_packets(&left, left_buffer, &right, right_buffer);

        fgets(left_buffer, MAX_CHARS, stdin); // skip a line
    }

    printf("there were %lu pairs with a sum of %lu\n", pairs, sum_of_right_ordered_indicies);
}