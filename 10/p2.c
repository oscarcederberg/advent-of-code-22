#include <stdlib.h>
#include <stdio.h>

#define MAX_CHARS 64
#define CRT_ROWS 6
#define CRT_COLS 40

struct cpu_t {
    int x;
    int cycle;
};

char get_pixel(struct cpu_t *cpu) {
    if (((cpu->cycle - 1) % CRT_COLS) >= cpu->x - 1 &&
        ((cpu->cycle - 1) % CRT_COLS) <= cpu->x + 1) {
        return '#';
    } else {
        return '.';
    }
}

int main(void) {
    struct cpu_t cpu = { .x = 1, .cycle = 0 };
    char crt[CRT_ROWS * CRT_COLS] = { 0, };
    char buffer[MAX_CHARS] = {0, };

    if (fgets(buffer, MAX_CHARS, stdin) == NULL) {
        exit(1);
    }

    do {
        cpu.cycle += 1;
        crt[cpu.cycle - 1] = get_pixel(&cpu);

        if (buffer[0] == 'n') {
            continue;
        }

        cpu.cycle += 1;
        crt[cpu.cycle - 1] = get_pixel(&cpu);
        cpu.x += atoi(&buffer[5]);
    } while(fgets(buffer, MAX_CHARS, stdin) != NULL);

    for (size_t i = 0; i < CRT_ROWS; i++) {
        printf("%.*s\n", CRT_COLS, &crt[i * CRT_COLS]);
    }
}