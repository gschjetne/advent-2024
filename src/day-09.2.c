#include "../lib/vector.h"
#include <stdio.h>

typedef struct entry_s {
    int file_id;
    int file_blocks;
    int free_blocks;
} entry;

void debug_print(vector *map_v) {
    size_t length = vector_length(map_v);
    entry *es = map_v->base;

    for (int i = 0; i < length; i++) {
        for (int j = 0; j < es[i].file_blocks; j++) {
            putchar(es[i].file_id + '0');
        }

        for (int j = 0; j < es[i].free_blocks; j++) {
            putchar('.');
        }
    }
    putchar('\n');
}

void compact(vector *map_v) {
    size_t length = vector_length(map_v);
    entry *es = map_v->base;

    for (size_t from_id = length - 1; from_id > 0; from_id--) {
        // Hack to account for array changing during
        // iteration, may improve later.
        int from_i = 0;
        while (es[from_i].file_id != from_id) {
            from_i++;
        }

        for (size_t to_i = 0; to_i < from_i; to_i++) {
            if (es[to_i].free_blocks >= es[from_i].file_blocks) {
                entry from = es[from_i], *to = &es[to_i],
                      *from_before = &es[from_i - 1];

                vector_delete(map_v, from_i);

                from_before->free_blocks += from.file_blocks + from.free_blocks;

                from.free_blocks = to->free_blocks - from.file_blocks;
                to->free_blocks = 0;

                vector_insert(map_v, to_i, &from);

                break;
            }
        }
    }
}

long checksum(vector *map_v) {
    size_t length = vector_length(map_v);
    entry *es = map_v->base;
    long pos = 0, result = 0;

    for (int i = 0; i < length; i++) {
        entry e = es[i];

        for (int j = 0; j < e.file_blocks; j++) {
            result += e.file_id * pos++;
        }

        pos += e.free_blocks;
    }

    return result;
}

int main(void) {
    vector map_v;
    vector_init(&map_v, sizeof(entry));

    int file_id = 0, total_space = 0;

    while (true) {
        int chr = getchar();

        if (chr < '0' || chr > '9') {
            break;
        };

        int file_blocks = chr - '0';
        int free_blocks = getchar() - '0';

        if (free_blocks < 0) {
            free_blocks = 0;
        }

        entry e = {.file_id = file_id++,
                   .file_blocks = file_blocks,
                   .free_blocks = free_blocks};

        vector_push(&map_v, &e);

        total_space += file_blocks + free_blocks;
    }

    compact(&map_v);

    printf("result = %ld\n", checksum(&map_v));

    vector_destroy(&map_v);
    return 0;
}
