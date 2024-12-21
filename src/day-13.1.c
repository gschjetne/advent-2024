#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 80
#define MAX_GAMES 512
#define MAX_PRESSES 100

typedef struct {
    int x;
    int y;
} coord;

typedef struct {
    coord a;
    coord b;
    coord prize;
} game;

int parse(game *g) {
    char line[MAX_LINE];
    int i = 0;

    while (fgets(line, MAX_LINE, stdin) != NULL) {
        char *c = line;
        if (memcmp("Button A:", line, 9) == 0) {
            assert(i < MAX_GAMES);

            c = strchr(c, 'X') + 1;
            g->a.x = atoi(c);
            c = strchr(c, 'Y') + 1;
            g->a.y = atoi(c);
        } else if (memcmp("Button B:", line, 9) == 0) {
            c = strchr(c, 'X') + 1;
            g->b.x = atoi(c);
            c = strchr(c, 'Y') + 1;
            g->b.y = atoi(c);
        } else if (memcmp("Prize:", line, 6) == 0) {
            c = strchr(c, 'X') + 2;
            g->prize.x = atoi(c);
            c = strchr(c, 'Y') + 2;
            g->prize.y = atoi(c);

            g++;
            i++;
        }
    }

    return i;
}

int min_tokens(game *g) {
    int min_cost = 0;
    coord ac = g->a;
    coord bc = g->b;
    coord prize = g->prize;

    for (int a = 0; a <= MAX_PRESSES; a++) {
        for (int b = 0; b <= MAX_PRESSES; b++) {
            if (a * ac.x + b * bc.x == prize.x &&
                a * ac.y + b * bc.y == prize.y) {
                int cost = 3 * a + b;
                if (min_cost == 0 || cost < min_cost) {
                    min_cost = cost;
                }
            }
        }
    }

    return min_cost;
}

int main(void) {
    game games[MAX_GAMES];
    int result = 0;
    int count = parse(games);

    for (int i = 0; i < count; i++) {
        result += min_tokens(&games[i]);
    }

    printf("result = %d\n", result);

    return 0;
}
