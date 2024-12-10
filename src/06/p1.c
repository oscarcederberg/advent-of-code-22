#include <stdio.h>
#include <limits.h>

#define PACKET_WIDTH 4

int is_start_of_packet(char frame[], size_t width) {
    char character;
    for (size_t i = 0; i < width; i++) {
        character = frame[i];
        for (size_t j = (i + 1); j < width; j++) {
            if (frame[j] == character) {
                return 0;
            }
        }
    }
    return 1;
}

int main(void) {
    size_t i;
    char frame[PACKET_WIDTH];

    for (i = 0; i < PACKET_WIDTH; i++) {
        frame[i] = fgetc(stdin);
    }

    for (i = PACKET_WIDTH; i < ULONG_MAX; i++) {
        if (is_start_of_packet(frame, PACKET_WIDTH)) {
            break;
        }

        frame[i % PACKET_WIDTH] = fgetc(stdin);
    }

    printf("first marker after character %lu\n", i);
    return 0;
}