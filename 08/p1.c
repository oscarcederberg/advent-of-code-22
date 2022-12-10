#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_CHARS 256

struct trees_t {
    size_t rows;
    size_t cols;
    int **matrix;
};

void print_trees(struct trees_t *trees) {
    for (size_t i = 0; i < trees->rows; i++) {
        for (size_t j = 0; j < trees->cols; j++) {
            printf("%d", trees->matrix[i][j]);
        }
        printf("\n");
    }
}

void free_trees(struct trees_t *trees) {
    for (size_t i = 0; i < trees->rows; i++) {
        free(trees->matrix[i]);
    }
    free(trees->matrix);
    free(trees);
}

void strip_line(char *line) {
    size_t i;

    size_t tallest_index;

    for (i = 0; i < MAX_CHARS; i++) {
        if (!isdigit(line[i])) {
            line[i] = '\0';
            break;
        }
    }
}

void add_row(struct trees_t *trees, char *buffer) {
    trees->matrix = (int **) realloc(trees->matrix, (trees->rows + 1) * sizeof(int *));
    trees->matrix[trees->rows] = (int *) calloc(trees->cols, sizeof(int));
    for (size_t i = 0; i < trees->cols; i++) {
        trees->matrix[trees->rows][i] = buffer[i] - '0';
    }
    trees->rows++;
}

int check_visibility(struct trees_t *trees, size_t row, size_t col) {
    size_t i;
    int height = trees->matrix[row][col];
    int visible = 1;

    for (i = 0; i < col; i++) {
        if (trees->matrix[row][i] >= height) {
            visible = 0;
        }
    }

    if (visible) {
        return 1;
    }
    visible = 1;

    for (i = col + 1; i < trees->cols; i++) {
        if (trees->matrix[row][i] >= height) {
            visible = 0;
        }
    }

    if (visible) {
        return 1;
    }
    visible = 1;

    for (i = 0; i < row; i++) {
        if (trees->matrix[i][col] >= height) {
            visible = 0;
        }
    }

    if (visible) {
        return 1;
    }
    visible = 1;

    for (i = row + 1; i < trees->rows; i++) {
        if (trees->matrix[i][col] >= height) {
            visible = 0;
        }
    }

    return visible;
}

size_t get_visible(struct trees_t *trees) {
    size_t row, col, num_visible;
    num_visible = 2 * (trees->rows + trees->cols - 2);
    for (row = 1; row < trees->rows - 1; row++) {
        for (col = 1; col < trees->cols - 1; col++) {
            if (check_visibility(trees, row, col)) {
                num_visible++;
            }
        }
    }

    return num_visible;
}

int main(void) {
    struct trees_t *trees = (struct trees_t *) calloc(1, sizeof(struct trees_t));
    char buffer[MAX_CHARS] = {0, };

    if (fgets(buffer, MAX_CHARS, stdin) == NULL) {
        free(trees);
        exit(1);
    }

    strip_line(buffer);
    trees->cols = strlen(buffer);

    do {
        strip_line(buffer);
        add_row(trees, buffer);
    } while(fgets(buffer, MAX_CHARS, stdin) != NULL);

    size_t num_visible = get_visible(trees);
    printf("there are %lu visible trees\n", num_visible);
    free_trees(trees);
}