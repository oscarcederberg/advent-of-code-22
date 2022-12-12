#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_CHARS 256

enum type {
    ERROR,
    NORMAL,
    START,
    END
};

struct square_t {
    int x;
    int y;
    int explored;
    int distance;
    int elevation;
    char letter;
    enum type type;
};

struct map_t {
    size_t rows;
    size_t cols;
    int start_x;
    int start_y;
    struct square_t **squares;
};

struct node_t {
    struct square_t *square;
    struct node_t *next;
};

void free_map(struct map_t* map) {
    for (size_t i = 0; i < map->rows * map->cols; i++) {
        free(map->squares[i]);
    }
    free(map->squares);
    free(map);
}

void free_queue(struct node_t* head) {
    struct node_t* node;
    while(head != NULL) {
        node = head->next;
        free(head);
        head = node;
    }
}

void strip(char *buffer) {
    for (size_t i = 0; i < MAX_CHARS; i++) {
        if(!isalpha(buffer[i])) {
            buffer[i] = '\0';
            break;
        }
    }
}

void parse_row(struct map_t *map, char *buffer) {
    struct square_t *square;

    for (size_t i = 0; i < map->cols; i++) {
        square = (struct square_t *) calloc(1, sizeof(struct square_t));
        square->x = i;
        square->y = map->rows - 1;
        square->letter = buffer[i];

        switch (buffer[i]) {
        case 'a':
        case 'S':
            square->elevation = 'a' - 'a';
            square->type = END;
            break;
        case 'E':
            square->elevation = 'z' - 'a';
            map->start_x = square->x;
            map->start_y = square->y;
            square->type = START;
            break;
        default:
            square->elevation = buffer[i] - 'a';
            square->type = NORMAL;
            break;
        }

        map->squares[square->y * map->cols + square->x] = square;
    }
}

void parse_map(struct map_t *map) {
    char buffer[MAX_CHARS] = {0, };

    if (fgets(buffer, MAX_CHARS, stdin) == NULL) {
        exit(1);
    }

    strip(buffer);
    map->cols = strlen(buffer);

    do {
        map->rows += 1;
        map->squares = (struct square_t **) realloc(map->squares, map->rows * map->cols * sizeof(struct square_t *));

        parse_row(map, buffer);
    } while (fgets(buffer, MAX_CHARS, stdin) != NULL);
}

void enqueue(struct node_t *queue, struct node_t *node) {
    while (queue->next != NULL) {
        queue = queue->next;
    }
    queue->next = node;
}

void check_and_enqueue(struct square_t *square, struct square_t* other, struct node_t *queue) {
    if (!other->explored && other->elevation >= (square->elevation - 1)) {
        other->explored = 1;
        other->distance = square->distance + 1;

        struct node_t *node = calloc(1, sizeof(struct node_t));
        node->square = other;
        node->next = NULL;
        enqueue(queue, node);
    }
}

int breadth_first_search_map(struct map_t *map) {
    struct square_t *square;
    struct node_t *head = calloc(1, sizeof(struct node_t));
    struct node_t *temp;
    head->square = map->squares[map->start_y * map->cols + map->start_x];
    head->square->distance = 0;
    head->next = NULL;

    while (head != NULL) {
        square = head->square;
        square->explored = 1;

        if (square->type == END) {
            free_queue(head);
            return square->distance;
        }

        if (square->x > 0) {
            check_and_enqueue(square, map->squares[square->y * map->cols + (square->x - 1)], head);
        }

        if (square->x < map->cols - 1) {
            check_and_enqueue(square, map->squares[square->y * map->cols + (square->x + 1)], head);
        }

        if (square->y > 0) {
            check_and_enqueue(square, map->squares[(square->y - 1) * map->cols + square->x], head);
        }

        if (square->y < map->rows - 1) {
            check_and_enqueue(square, map->squares[(square->y + 1) * map->cols + square->x], head);
        }

        temp = head;
        head = head->next;
        free(temp);
    }

    exit(1);
}

int main(void) {
    struct map_t *map = calloc(1, sizeof(struct map_t));

    parse_map(map);
    int distance = breadth_first_search_map(map);

    printf("the shortest distance to the end is %d\n", distance);
    free_map(map);
}