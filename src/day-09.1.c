#include "../lib/vector.h"
#include <stdio.h>
#include <stdlib.h>

const int FREE = -1;

int main(void) {
    vector map_v;
    vector_init(&map_v, sizeof(int));

    int file_id = 0;

    while (true) {
        int chr = getchar();

        if (chr < '0' || chr > '9') {
            break;
        };

        int file_blocks = chr - '0';
        int gap_blocks = getchar() - '0';

        for (int i = 0; i < file_blocks; i++) {
            vector_push(&map_v, &file_id);
        }

        for (int i = 0; i < gap_blocks; i++) {
            vector_push(&map_v, &FREE);
        }

        file_id++;
    }

    int *map = map_v.base;

    for (int i = 0; i < vector_length(&map_v); i++) {
        if (map[i] == FREE) {
            int move_id;

            do {
                vector_pop(&map_v, &move_id);
            } while (move_id == -1);

            map[i] = move_id;
        }
    }

    size_t final_length = vector_length(&map_v);

    long result = 0;

    for (int pos = 0; pos < final_length; pos++) {
        if (pos != FREE) {
            result += pos * map[pos];
        }
    }

    printf("result = %ld\n", result);

    vector_destroy(&map_v);
    return 0;
}
