#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int get_scenic_score(struct trees_t *trees, size_t row, size_t col) {
    size_t i;
    size_t score = 1;

    int height = trees->matrix[row][col];

    for (i = 1; i < col; i++) {
        if (trees->matrix[row][col - i] >= height) {
            break;
        }
    }
    score *= i;

    for (i = 1; i < trees->cols - (col + 1); i++) {
        if (trees->matrix[row][col + i] >= height) {
            break;
        }
    }
    score *= i;

    for (i = 1; i < row; i++) {
        if (trees->matrix[row - i][col] >= height) {
            break;
        }
    }
    score *= i;

    for (i = 1; i < trees->rows - (row + 1); i++) {
        if (trees->matrix[row + i][col] >= height) {
            break;
        }
    }
    score *= i;

    return score;
}

size_t get_highest_scenic_score(struct trees_t *trees) {
    size_t row, col, highest_scenic_score, scenic_score;
    highest_scenic_score = 0;

    for (row = 1; row < trees->rows - 1; row++) {
        for (col = 1; col < trees->cols - 1; col++) {
            scenic_score = get_scenic_score(trees, row, col);
            if (highest_scenic_score < scenic_score) {
                highest_scenic_score = scenic_score;
            }
        }
    }

    return highest_scenic_score;
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

    size_t highest_scenic_score = get_highest_scenic_score(trees);
    printf("the highest scenic score is %lu\n", highest_scenic_score);
    free_trees(trees);
}