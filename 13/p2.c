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
    int is_divider;
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

int compare_pair(const void *first, const void *second) {
    size_t index = 0;
    struct value_t *temp;
    struct value_t *left = *((struct value_t **) first);
    struct value_t *right = *((struct value_t **) second);

    if (left->count == 0 && right->count == 0) {
        return 1;
    }

    while (1) {
        if (index == left->count && index == right->count) {
            left = left->parent;
            right = right->parent;

            if (left == NULL) {
                return -1;
            } else if (right == NULL) {
                return 1;
            }

            if (left->value != right->value) {
                exit(1);
            }
            index = left->value;
            continue;
        } if (index == left->count) {
            return -1;
        } else if (index == right->count) {
            return 1;
        }

        for (index = index; index < left->count && index < right->count; index++) {
            if (left->values[index]->type == NUMBER && right->values[index]->type == NUMBER) {
                if (left->values[index]->value != right->values[index]->value) {
                    if (left->values[index]->value < right->values[index]->value) {
                        return -1;
                    } else {
                        return 1;
                    }
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
    size_t product_of_divider_indicies = 1;
    size_t num_packets = 2;
    struct value_t **packets = calloc(num_packets, sizeof(struct value_t *));
    struct value_t *left = calloc(1, sizeof(struct value_t));
    struct value_t *right = calloc(1, sizeof(struct value_t));
    char left_buffer[MAX_CHARS] = {0, };
    char right_buffer[MAX_CHARS] = {0, };

    left->type = LIST;
    left->is_divider = 1;
    left->count = 1;
    left->values = calloc(1, sizeof(struct value_t *));
    left->values[0] = calloc(1, sizeof(struct value_t));
    left->values[0]->type = NUMBER;
    left->values[0]->value = 2;

    right->type = LIST;
    right->is_divider = 1;
    right->count = 1;
    right->values = calloc(1, sizeof(struct value_t *));
    right->values[0] = calloc(1, sizeof(struct value_t));
    right->values[0]->type = NUMBER;
    right->values[0]->value = 6;

    packets[0] = left;
    packets[1] = right;

    while (fgets(left_buffer, MAX_CHARS, stdin) != NULL &&
        fgets(right_buffer, MAX_CHARS, stdin) != NULL) {
        num_packets += 2;
        packets = (struct value_t **) realloc(packets, num_packets * sizeof(struct void_t *));

        left = parse_packet(left_buffer);
        right = parse_packet(right_buffer);
        packets[num_packets - 2] = left;
        packets[num_packets - 1] = right;

        fgets(left_buffer, MAX_CHARS, stdin); // skip a line
    }

    qsort(packets, num_packets, sizeof(struct value_t *), compare_pair);

    for (size_t i = 0; i < num_packets; i++) {
        if (packets[i]->is_divider) {
            product_of_divider_indicies *= (i + 1);
        }
        free_packet(packets[i]);
    }
    free(packets);

    printf("the decoder key for the distress signal is %lu\n", product_of_divider_indicies);
}