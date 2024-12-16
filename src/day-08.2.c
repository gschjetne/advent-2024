#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int x;
    int y;
} coord;

typedef struct {
    int antenna_count;
    coord coordinates[5];
} freq;

void add_antenna(freq *f, int x, int y) {
    assert(f->antenna_count < 5);
    coord *c = &f->coordinates[f->antenna_count];
    c->x = x;
    c->y = y;
    f->antenna_count++;
}

bool is_antinode_of(freq *f, int x, int y) {
    int count = f->antenna_count;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (i == j) {
                continue;
            }

            coord ci = f->coordinates[i];
            coord cj = f->coordinates[j];

            int dxi = ci.x - x;
            int dyi = ci.y - y;

            int dxj = cj.x - x;
            int dyj = cj.y - y;

            if (dxi * dyj - dyi * dxj == 0) {
                return true;
            }
        }
    }

    return false;
}

int main(void) {
    int result = 0;
    freq freqs['z' - '0'];
    memset(freqs, 0, sizeof(freqs));

    int chr, x = 0, y = 0;
    while ((chr = getchar()) != EOF) {
        if (chr == '\n') {
            x = 0;
            y++;
            continue;
        }

        if (chr >= '0' && chr <= 'z') {
            add_antenna(&freqs[chr - '0'], x, y);
        }

        x++;
    }

    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 50; x++) {
            for (int f = 0; f < 'z' - '0'; f++) {
                if (is_antinode_of(&freqs[f], x, y)) {
                    result++;
                    break;
                }
            }
        }
    }

    printf("result = %d\n", result);

    return 0;
}
