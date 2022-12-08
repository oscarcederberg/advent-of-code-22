#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_ROWS 64

struct crates_t {
    size_t rows;
    size_t cols;
    char** matrix;
};

void print_crates(struct crates_t *crates) {
    char character;
    printf("%lu rows %lu cols\n", crates->rows, crates->cols);
    for (size_t i = 0; i < crates->rows; i++){
        for (size_t j = 0; j < crates->cols; j++){
            character = crates->matrix[i][j];
            if (character == ' ') {
                printf("    ");
            } else {
                printf("[%c] ", character);
            }
        }
        printf("\n");
    }
}

void free_crates(struct crates_t *crates) {
    for (size_t i = 0; i < crates->rows; i++) {
        free(crates->matrix[i]);
    }
    free(crates->matrix);
    free(crates);
}

void reverse_rows(struct crates_t *crates) {
    char *temp;

    int mid = crates->rows / 2;
    for (size_t i = 0; i < mid; i++) {
        temp = crates->matrix[crates->rows - (i + 1)];
        crates->matrix[crates->rows - (i + 1)] = crates->matrix[i];
        crates->matrix[i] = temp;
    }
}

size_t scan_row_return_amount(char **row, int perform_realloc) {
    char character;
    size_t crates_per_line = 0;
    size_t chars_per_crate = 5;
    char buffer[chars_per_crate];

    do {
        crates_per_line++;
        if (perform_realloc) {
            *row = realloc(*row, crates_per_line * sizeof(char));
        }

        fgets(buffer, chars_per_crate, stdin);

        if (buffer[1] == ' ') {
            (*row)[crates_per_line - 1] = ' ';
        } else if (isalpha(buffer[1])) {
            (*row)[crates_per_line - 1] = buffer[1];
        } else {
            return 0;
        }
    } while (buffer[3] != '\n');

    return crates_per_line;
}

void scan_crates(struct crates_t *crates) {
    size_t i, j;
    char* temp_row = NULL;

    crates->matrix = (char **) calloc(1, sizeof(char *));

    j = scan_row_return_amount(&temp_row, 1);
    crates->matrix[0] = (char *) calloc(j, sizeof(char));
    memcpy(crates->matrix[0], temp_row, j * sizeof(char));

    for (i = 1; i < MAX_ROWS; i++){
        if (scan_row_return_amount(&temp_row, 0) == 0) {
            break;
        }

        crates->matrix = (char **) realloc(crates->matrix, (i + 1) * sizeof(char *));
        crates->matrix[i] = (char *) calloc(j, sizeof(char));
        memcpy(crates->matrix[i], temp_row, j * sizeof(char));
    }

    crates->rows = i;
    crates->cols = j;
    reverse_rows(crates);

    free(temp_row);
}

void extend_matrix(struct crates_t *crates, size_t to) {
    crates->matrix = (char **) realloc(crates->matrix, to * sizeof(char *));
    for (size_t i = crates->rows; i < to; i++){
        crates->matrix[i] = (char *) calloc(crates->cols, sizeof(char *));
        for (size_t j = 0; j < crates->cols; j++) {
            crates->matrix[i][j] = ' ';
        }
    }

    crates->rows = to;
}

int get_top_index_of_col(struct crates_t *crates, int col) {
    for (int i = 0; i < crates->rows; i++) {
        if (crates->matrix[i][col] == ' ') {
            return i - 1;
        }
    }
}

void move(struct crates_t *crates, int amount, int from, int to) {
    int i;

    int from_top = get_top_index_of_col(crates, from);
    int to_top = get_top_index_of_col(crates, to);

    for (i = 0; i < amount; i ++) {
        crates->matrix[to_top + 1][to] = crates->matrix[from_top][from];
        crates->matrix[from_top][from] = ' ';
        from_top--;
        to_top++;
    }
}

void perform_instructions(struct crates_t *crates) {
    int amount, from, to;

    extend_matrix(crates, crates->rows * crates->cols);

    while (scanf(" move %d from %d to %d", &amount, &from, &to) == 3) {
        move(crates, amount, from - 1, to - 1);
    }
}

void print_top_of_each_stack(struct crates_t *crates) {
    int top;
    char character;

    printf("the crates on top of each stack are ");
    for (size_t i = 0; i < crates->cols; i++) {
        top = get_top_index_of_col(crates, i);
        character = crates->matrix[top][i];
        printf("%c", character);
    }
    printf("\n");
}

int main() {
    struct crates_t *crates = (struct crates_t *) calloc(1, sizeof(struct crates_t));

    scan_crates(crates);
    scanf("%*[^\n]\n"); // skip a line
    perform_instructions(crates);
    print_top_of_each_stack(crates);

    free_crates(crates);
}