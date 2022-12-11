#include <stdlib.h>
#include <stdio.h>

#define MAX_CHARS 64
#define ITERATIONS 20

enum type {
    ERR,
    ADD,
    MUL,
    POW
};

struct operation_t {
    enum type type;
    int term;
};

struct monkey_t {
    size_t num_items;
    int *items;
    int num_inspected;
    struct operation_t operation;
    int test_divisible_by;
    int throw_to_if_true;
    int throw_to_if_false;
};

void print_monkey(struct monkey_t *monkey) {
    printf("monkey \n\titems: ");
    for (size_t i = 0; i < monkey->num_items; i++) {
        printf("%d ", monkey->items[i]);
    }

    printf("\n\toperation: new = old ");
    switch (monkey->operation.type) {
    case ADD:
        printf("+ %d\n", monkey->operation.term);
        break;
    case MUL:
        printf("* %d\n", monkey->operation.term);
        break;
    case POW:
        printf("* old\n");
        break;
    default:
        break;
    }

    printf("\ttest divisible by %d\n", monkey->test_divisible_by);
    printf("\t\tthrow to %d if true\n", monkey->throw_to_if_true);
    printf("\t\tthrow to %d if false\n\n", monkey->throw_to_if_false);
}

void parse_monkeys(struct monkey_t*** monkeys, size_t *num_monkeys) {
    size_t i;
    int temp_item;
    struct monkey_t *current_monkey;
    char buffer[MAX_CHARS] = {0, };

    while(fgets(buffer, MAX_CHARS, stdin) != NULL) {
        *monkeys = (struct monkey_t **) realloc(*monkeys, (*num_monkeys + 1) * sizeof(struct monkey_t *));
        (*monkeys)[*num_monkeys] = (struct monkey_t *) calloc(1, sizeof(struct monkey_t));
        current_monkey = (*monkeys)[*num_monkeys];

        scanf("\tStarting items: %d", &temp_item);
        current_monkey->num_items++;
        current_monkey->items = (int *) calloc(current_monkey->num_items, sizeof(int));
        current_monkey->items[0] = temp_item;
        for (i = 1; scanf(", %d", &temp_item) != 0; i++) {
            current_monkey->num_items++;
            current_monkey->items = (int *) realloc(current_monkey->items, current_monkey->num_items * sizeof(int));
            current_monkey->items[i] = temp_item;
        }
        fgets(buffer, MAX_CHARS, stdin); // skip current line

        fgets(buffer, MAX_CHARS, stdin);
        if (buffer[23] == '+') {
            current_monkey->operation.type = ADD;
            current_monkey->operation.term = atoi(&buffer[24]);
        } else if (buffer[25] != 'o') {
            current_monkey->operation.type = MUL;
            current_monkey->operation.term = atoi(&buffer[24]);
        } else {
            current_monkey->operation.type = POW;
            current_monkey->operation.term = 0;
        }

        fgets(buffer, MAX_CHARS, stdin);
        current_monkey->test_divisible_by = atoi(&buffer[21]);

        fgets(buffer, MAX_CHARS, stdin);
        current_monkey->throw_to_if_true = atoi(&buffer[29]);

        fgets(buffer, MAX_CHARS, stdin);
        current_monkey->throw_to_if_false = atoi(&buffer[30]);

        fgets(buffer, MAX_CHARS, stdin);
        *num_monkeys += 1;
    }
}

void monkey_perform(struct monkey_t **monkeys, int index) {
    int temp_item;
    struct monkey_t *throw_to;
    struct monkey_t *current_monkey = monkeys[index];

    for (size_t i = 0; i < current_monkey->num_items; i++) {
        temp_item = current_monkey->items[i];
        switch (current_monkey->operation.type) {
        case ADD:
            temp_item += current_monkey->operation.term;
            break;
        case MUL:
            temp_item *= current_monkey->operation.term;
            break;
        case POW:
            temp_item *= temp_item;
            break;
        default:
            break;
        }

        temp_item /= 3;

        if (temp_item % current_monkey->test_divisible_by) {
            throw_to = monkeys[current_monkey->throw_to_if_false];
        } else {
            throw_to = monkeys[current_monkey->throw_to_if_true];
        }
        throw_to->num_items++;
        throw_to->items =(int *) realloc(throw_to->items, throw_to->num_items * sizeof(int));
        throw_to->items[throw_to->num_items - 1] = temp_item;

        current_monkey->num_inspected++;
    }

    current_monkey->num_items = 0;
    free(current_monkey->items);
    current_monkey->items = NULL;
}

void perform_monkey_business(struct monkey_t **monkeys, size_t num_monkeys, size_t iterations) {
    size_t i, j, k;

    for (i = 0; i < iterations; i++) {
        for (j = 0; j < num_monkeys; j++) {
            monkey_perform(monkeys, j);
        }
    }
}

int calculate_monkey_business(struct monkey_t **monkeys, size_t num_monkeys) {
    int highest_1 = 0, highest_2 = 0, inspected;

    for (size_t i = 0; i < num_monkeys; i++) {
        inspected = monkeys[i]->num_inspected;
        if (inspected >= highest_1) {
            highest_2 = highest_1;
            highest_1 = inspected;
        } else if (inspected >= highest_2) {
            highest_2 = inspected;
        }
    }

    return highest_1 * highest_2;
}

int main (void) {
    size_t i;
    size_t num_monkeys = 0;
    struct monkey_t **monkeys = NULL;

    parse_monkeys(&monkeys, &num_monkeys);
    perform_monkey_business(monkeys, num_monkeys, ITERATIONS);
    int level_of_monkey_business = calculate_monkey_business(monkeys, num_monkeys);

    printf("after %d rounds of stuff-slinging simian shenanigans the level of monkey business was %d\n", ITERATIONS, level_of_monkey_business);
}