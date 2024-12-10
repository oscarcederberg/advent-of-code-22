#include <stdlib.h>
#include <stdio.h>

#define MAX_CHARS 64

struct cpu_t {
    int x;
    int cycle;
};

int main(void) {
    struct cpu_t cpu = { .x = 1, .cycle = 0 };
    char buffer[MAX_CHARS] = {0, };
    int signal_strength_sum;

    if (fgets(buffer, MAX_CHARS, stdin) == NULL) {
        exit(1);
    }

    do {
        cpu.cycle += 1;
        if (((cpu.cycle - 20) % 40) == 0) {
            signal_strength_sum += cpu.cycle * cpu.x;
        }

        if (buffer[0] == 'n') {
            continue;
        }

        cpu.cycle += 1;
        if (((cpu.cycle - 20) % 40) == 0) {
            signal_strength_sum += cpu.cycle * cpu.x;
        }
        cpu.x += atoi(&buffer[5]);
    } while(fgets(buffer, MAX_CHARS, stdin) != NULL);

    printf("the sum of the program's signal strengths is %i\n", signal_strength_sum);
}