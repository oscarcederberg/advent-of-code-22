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
    int length;
    int *segment_x;
    int *segment_y;
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
    free(rope_state->segment_x);
    free(rope_state->segment_y);
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
        break;
    case 'D':
        *direction = DOWN;
        break;
    case 'L':
        *direction = LEFT;
        break;
    case 'R':
        *direction = RIGHT;
        break;
    default:
        *direction = ERROR;
        break;
    }

    *steps = atoi(&buffer[2]);
}

int segments_touching(int x0, int y0, int x1, int y1) {
    return !(x0 < x1 - 1 || x0 > x1 + 1 || y0 < y1 - 1 || y0 > y1 + 1);
}

void perform_command(struct rope_state_t *rope_state, enum direction direction, int steps) {
    size_t i,j;
    int x0, y0, x1, y1;

    for (i = 0; i < steps; i++) {
        switch (direction) {
        case UP:
            rope_state->segment_y[0] += 1;
            break;
        case DOWN:
            rope_state->segment_y[0] -= 1;
            break;
        case LEFT:
            rope_state->segment_x[0] -= 1;
            break;
        case RIGHT:
            rope_state->segment_x[0] += 1;
            break;
        default:
            exit(1);
            break;
        }

        for (j = 1; j < rope_state->length; j++) {
            x0 = rope_state->segment_x[j - 1];
            y0 = rope_state->segment_y[j - 1];
            x1 = rope_state->segment_x[j];
            y1 = rope_state->segment_y[j];

            if (segments_touching(x0, y0, x1, y1)) {
                continue;
            } else if (x0 == x1) {
                rope_state->segment_y[j] += (y0 > y1 ? 1 : -1);
            } else if (y0 == y1) {
                rope_state->segment_x[j] += (x0 > x1 ? 1 : -1);
            } else {
                rope_state->segment_x[j] += (x0 > x1 ? 1 : -1);
                rope_state->segment_y[j] += (y0 > y1 ? 1 : -1);
            }
        }

        if (rope_state->segment_x[0] >= rope_state->cols || rope_state->segment_y[0] >= rope_state->rows) {
            for (j = 0; j < rope_state->length; j++) {
                printf("%lu : (%d, %d)\n", j, rope_state->segment_x[j], rope_state->segment_y[j]);
            }
            printf("out of range\n");
            exit(1);
        }
        if (!rope_state->matrix[rope_state->segment_y[rope_state->length - 1]][rope_state->segment_x[rope_state->length - 1]]) {
            rope_state->matrix[rope_state->segment_y[rope_state->length - 1]][rope_state->segment_x[rope_state->length - 1]] = 1;
            rope_state->visited++;
        }
    }
}

int main(void) {
    size_t i;

    struct rope_state_t *rope_state = (struct rope_state_t *) calloc(1, sizeof(struct rope_state_t));
    enum direction direction = 0;
    int steps = 0;
    char buffer[MAX_CHARS] = {0, };

    rope_state->rows = MATRIX_ROWS;
    rope_state->cols = MATRIX_COLS;
    rope_state->length = 10;
    rope_state->segment_x = (int *) calloc(rope_state->length, sizeof(int));
    rope_state->segment_y = (int *) calloc(rope_state->length, sizeof(int));
    for (i = 0; i < rope_state->length; i++) {
        rope_state->segment_x[i] = MATRIX_COLS / 2;
        rope_state->segment_y[i] = MATRIX_ROWS / 2;
    }
    rope_state->matrix = (int **) calloc(rope_state->rows, sizeof(int *));

    for (i = 0; i < rope_state->rows; i++) {
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