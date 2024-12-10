#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_CHARS 516

enum value_type {
    ERROR,
    LIST,
    NUMBER
};

struct value_t {
    struct value_t *parent;
    enum value_type type;
    int value;
    int count;
    struct value_t **values;
};

void free_packet(struct value_t *packet) {
    for (size_t i = 0; i < packet->count; i++) {
        free_packet(packet->values[i]);
    }
    free(packet);
}

void print_list(struct value_t *list) {
    struct value_t *temp;
    for (size_t i = 0; i < list->count; i++) {
        temp = list->values[i];
        switch (temp->type) {
        case LIST:
            printf("[");
            print_list(temp);
            printf("]");
            break;
        case NUMBER:
            printf("%d", temp->value);
            break;
        default:
            printf("ERROR");
            break;
        }

        if (i < list->count - 1) {
            printf(",");
        }
    }
}

void print_packet(struct value_t *list) {
    switch (list->type) {
    case LIST:
        printf("[");
        print_list(list);
        printf("]");
        break;
    case NUMBER:
        printf("%d", list->value);
        break;
    default:
        printf("ERROR");
        break;
    }
    printf("\n");
}

struct value_t *parse_packet(char *buffer) {
    char character;
    struct value_t* packet = calloc(1, sizeof(struct value_t));
    struct value_t* temp = NULL;

    packet->type = LIST;

    for (size_t i = 1; i < MAX_CHARS; i++) {
        temp = NULL;
        character = buffer[i];
        switch (character) {
        case '[':
            temp = (struct value_t *) calloc(1, sizeof(struct value_t));
            temp->parent = packet;
            temp->type = LIST;
            break;
        case ']':
            if (packet->parent == NULL) {
                return packet;
            }

            packet = packet->parent;
            break;
        case ',':
            break;
        default:
            if(isdigit(character)) {
                temp = (struct value_t *) calloc(1, sizeof(struct value_t));
                temp->parent = packet;
                temp->type = NUMBER;
                temp->value = atoi(&buffer[i]);
                while (isdigit(buffer[i + 1])){
                    i++;
                }
            }
            break;
        }

        if (temp == NULL) {
            continue;
        }

        packet->count++;
        packet->values = (struct value_t **) realloc(packet->values, packet->count * sizeof(struct value_t *));
        packet->values[packet->count - 1] = temp;

        if (temp->type == LIST) {
            packet = temp;
        }
    }
    return NULL;
}

int compare_pair(struct value_t *left, struct value_t *right) {
    size_t index = 0;
    struct value_t *temp;

    if (left->count == 0 && right->count == 0) {
        return 1;
    }

    while (1) {
        if (index == left->count && index == right->count) {
            left = left->parent;
            right = right->parent;

            if (left == NULL) {
                return 1;
            } else if (right == NULL) {
                return 0;
            }

            if (left->value != right->value) {
                exit(1);
            }
            index = left->value;
            continue;
        } if (index == left->count) {
            return 1;
        } else if (index == right->count) {
            return 0;
        }

        for (index = index; index < left->count && index < right->count; index++) {
            if (left->values[index]->type == NUMBER && right->values[index]->type == NUMBER) {
                if (left->values[index]->value != right->values[index]->value) {
                    return left->values[index]->value < right->values[index]->value;
                }
                continue;
            } else if (left->values[index]->type != LIST) {
                temp = (struct value_t *) calloc(1, sizeof(struct value_t));
                temp->parent = left;
                temp->type = LIST;
                temp->count = 1;
                temp->values = (struct value_t **) calloc(1, sizeof(struct value_t *));
                left->values[index]->parent = temp;
                temp->values[0] = left->values[index];
                left->values[index] = temp;
            } else if (right->values[index]->type != LIST){
                temp = (struct value_t *) calloc(1, sizeof(struct value_t));
                temp->parent = right;
                temp->type = LIST;
                temp->count = 1;
                temp->values = (struct value_t **) calloc(1, sizeof(struct value_t *));
                right->values[index]->parent = temp;
                temp->values[0] = right->values[index];
                right->values[index] = temp;
            }

            left->value = index + 1;
            right->value = index + 1;
            left = left->values[index];
            right = right->values[index];
            index= 0;
            break;
        }
    }
}

int main(void) {
    size_t sum_of_right_ordered_indicies = 0;
    size_t pairs = 0;
    struct value_t *left;
    struct value_t *right;
    char left_buffer[MAX_CHARS] = {0, };
    char right_buffer[MAX_CHARS] = {0, };

    while (fgets(left_buffer, MAX_CHARS, stdin) != NULL &&
        fgets(right_buffer, MAX_CHARS, stdin) != NULL) {
        pairs++;

        left = parse_packet(left_buffer);
        right = parse_packet(right_buffer);
        if (compare_pair(left, right)) {
            sum_of_right_ordered_indicies += pairs;
        }

        free_packet(left);
        free_packet(right);
        fgets(left_buffer, MAX_CHARS, stdin); // skip a line
    }

    printf("there were %lu pairs with a sum of %lu\n", pairs, sum_of_right_ordered_indicies);
}