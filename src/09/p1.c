#include <stdlib.h>
#include <stdio.h>

#define MAX_CHARS 64
#define MATRIX_ROWS 1024
#define MATRIX_COLS 1024

enum direction {
    ERROR,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct rope_state_t {
    int head_x;
    int head_y;
    int tail_x;
    int tail_y;
    size_t visited;
    size_t rows;
    size_t cols;
    int **matrix;
};

void free_rope_state(struct rope_state_t *rope_state) {
    for (size_t i = 0; i < rope_state->cols; i++) {
        free(rope_state->matrix[i]);
    }
    free(rope_state->matrix);
    free(rope_state);
}

void allocate_matrix(struct rope_state_t *rope_state, size_t new_rows, size_t new_cols) {
    size_t i, j;
    rope_state->matrix = (int **) realloc(rope_state->matrix, new_rows * sizeof(int *));

    for (i = 0; i < rope_state->cols; i++) {
        rope_state->matrix[i] = (int *) realloc(rope_state->matrix[i], new_cols * sizeof(int));
    }

    for (i = 0; i < rope_state->rows; i++) {
        for (j = rope_state->cols; i < new_cols; j++) {
            rope_state->matrix[i][j] = 0;
        }
    }

    for (i = rope_state->rows; i < new_rows; i++) {
        for (j = 0; i < new_cols; j++) {
            rope_state->matrix[i][j] = 0;
        }
    }

    rope_state->rows = new_rows;
    rope_state->cols = new_cols;
}

void parse_command(char *buffer, enum direction *direction, int *steps) {
    switch (buffer[0]) {
    case 'U':
        *direction = UP;
        printf("up ");
        break;
    case 'D':
        *direction = DOWN;
        printf("down ");
        break;
    case 'L':
        *direction = LEFT;
        printf("left ");
        break;
    case 'R':
        *direction = RIGHT;
        printf("right ");
        break;
    default:
        *direction = ERROR;
        printf("ERROR ");
        break;
    }

    *steps = atoi(&buffer[2]);
    printf("%i steps\n", *steps);
}

void perform_command(struct rope_state_t *rope_state, enum direction direction, int steps) {
    for (size_t i = 0; i < steps; i++) {
        switch (direction) {
        case UP:
            rope_state->head_y += 1;
            if (rope_state->head_y > rope_state->tail_y + 1) {
                rope_state->tail_y = rope_state->head_y - 1;
                rope_state->tail_x = rope_state->head_x;
            }
            break;
        case DOWN:
            rope_state->head_y -= 1;
            if (rope_state->head_y < rope_state->tail_y - 1) {
                rope_state->tail_y = rope_state->head_y + 1;
                rope_state->tail_x = rope_state->head_x;
            }
            break;
        case LEFT:
            rope_state->head_x -= 1;
            if (rope_state->head_x < rope_state->tail_x - 1) {
                rope_state->tail_x = rope_state->head_x + 1;
                rope_state->tail_y = rope_state->head_y;
            }
            break;
        case RIGHT:
            rope_state->head_x += 1;
            if (rope_state->head_x > rope_state->tail_x + 1) {
                rope_state->tail_x = rope_state->head_x - 1;
                rope_state->tail_y = rope_state->head_y;
            }
            break;
        default:
            exit(1);
            break;
        }

        if (rope_state->head_x >= rope_state->cols || rope_state->head_y >= rope_state->rows) {
            exit(1);
        }
        if (!rope_state->matrix[rope_state->tail_y][rope_state->tail_x]) {
            rope_state->matrix[rope_state->tail_y][rope_state->tail_x] = 1;
            rope_state->visited++;
        }
    }
    printf("H : (%d, %d)\n", rope_state->head_x, rope_state->head_y);
    printf("T : (%d, %d)\n", rope_state->tail_x, rope_state->tail_y);
}

int main(void) {
    struct rope_state_t *rope_state = (struct rope_state_t *) calloc(1, sizeof(struct rope_state_t));
    enum direction direction = 0;
    int steps = 0;
    char buffer[MAX_CHARS] = {0, };

    rope_state->rows = MATRIX_ROWS;
    rope_state->cols = MATRIX_COLS;
    rope_state->head_y = MATRIX_ROWS / 2;
    rope_state->head_x = MATRIX_COLS / 2;
    rope_state->tail_y = MATRIX_ROWS / 2;
    rope_state->tail_x = MATRIX_COLS / 2;
    rope_state->matrix = (int **) calloc(rope_state->rows, sizeof(int *));

    for (size_t i = 0; i < rope_state->rows; i++) {
        rope_state->matrix[i] = (int *) calloc(rope_state->cols, sizeof(int));
    }

    if (fgets(buffer, MAX_CHARS, stdin) == NULL) {
        free_rope_state(rope_state);
        exit(1);
    }

    do {
        parse_command(buffer, &direction, &steps);
        perform_command(rope_state, direction, steps);
    } while(fgets(buffer, MAX_CHARS, stdin) != NULL);

    printf("the tail visited %lu positions\n", rope_state->visited);
    free_rope_state(rope_state);
}