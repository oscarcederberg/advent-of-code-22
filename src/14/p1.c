#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MATRIX_ROWS 258
#define MATRIX_COLS 512

enum state {
    AIR,
    ROCK,
    SAND
};

struct boundary_t {
    unsigned int min_col;
    unsigned int min_row;
    unsigned int max_col;
    unsigned int max_row;
};

void print_plane(enum state *plane, struct boundary_t *bounds) {
    enum state state;
    for (size_t y = bounds->min_row; y <= bounds->max_row; y++) {
        for (size_t x = bounds->min_col; x <= bounds->max_col; x++) {
            state = plane[x + y * MATRIX_COLS];
            if (state == AIR) {
                printf(".");
            } else if (state == ROCK) {
                printf("#");
            } else if (state == SAND) {
                printf("o");
            }
        }
        printf("\n");
    }
}

void set(enum state *plane, unsigned int x, unsigned int y, enum state state) {
    //if (state == ROCK) printf("(%u,%u)\n", x, y);
    plane[x + MATRIX_COLS * y] = state;
}

enum state get(enum state *plane, unsigned int x, unsigned int y) {
    return plane[x + MATRIX_COLS * y];
}

void from_fill_to(enum state *plane, unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1) {
    if (x0 < x1) {
        for (size_t i = x0; i <= x1; i++) {
            set(plane, i, y0, ROCK);
        }
    } else if (x0 > x1) {
        for (size_t i = x0; i >= x1; i--) {
            set(plane, i, y0, ROCK);
        }
    } else if (y0 < y1) {
        for (size_t i = y0; i <= y1; i++) {
            set(plane, x0, i, ROCK);
        }
    } else if (y0 > y1) {
        for (size_t i = y0; i >= y1; i--) {
            set(plane, x0, i, ROCK);
        }
    }
}

void update_bounds(struct boundary_t *bounds, unsigned int x, unsigned int y) {
    if (x < bounds->min_col) {
        bounds->min_col = x;
    } else if (x > bounds->max_col) {
        bounds->max_col = x;
    }
    if (y < bounds->min_row) {
        bounds->min_row = y;
    } else if (y > bounds->max_row) {
        bounds->max_row = y;
    }
}

void parse_plane(enum state *plane, struct boundary_t *bounds) {
    unsigned int x0, y0, x1, y1;

    if (scanf("%u,%u", &x0, &y0) != 2) {
        exit(1);
    }

    bounds->min_col = x0;
    bounds->min_row = y0;
    bounds->max_col = x0;
    bounds->max_row = y0;

    do {
        set(plane, x0, y0, ROCK);
        update_bounds(bounds, x0, y0);
        while (scanf(" -> %u,%u", &x1, &y1) == 2) {
            from_fill_to(plane, x0, y0, x1, y1);
            update_bounds(bounds, x1, y1);
            x0 = x1;
            y0 = y1;
        }
    } while (scanf("%u,%u", &x0, &y0) == 2);
}

unsigned int simulate_sand(enum state *plane, struct boundary_t *bounds) {
    unsigned int x, y, units_of_sand = 0;

    while (1) {
        x = 500;
        y = 0;

        while (1) {
            if (y == bounds->max_row + 2) {
                return units_of_sand;
            }

            if (get(plane, x, y + 1) == AIR) {
                y += 1;
            } else if (get(plane, x - 1, y + 1) == AIR) {
                y += 1;
                x -= 1;
            } else if (get(plane, x + 1, y + 1) == AIR) {
                y += 1;
                x += 1;
            } else {
                units_of_sand += 1;
                set(plane, x, y, SAND);
                break;
            }
        }
    }
}

int main(void) {
    enum state plane[MATRIX_ROWS * MATRIX_COLS] = { AIR, };
    struct boundary_t bounds;

    parse_plane(plane, &bounds);
    bounds.min_row = 0;
    print_plane(plane, &bounds);
    unsigned int units_of_sand = simulate_sand(plane, &bounds);

    printf("%u units of sand came to rest\n", units_of_sand);
}