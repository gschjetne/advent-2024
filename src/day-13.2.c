#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 80
#define MAX_GAMES 512
#define MAX_PRESSES 1000000
#define PRIZE_OFFSET 10000000000000

typedef struct {
    long x;
    long y;
} coord;

typedef struct {
    coord a;
    coord b;
    coord prize;
} game;

void transform(double matrix[2][2], double *x, double *y) {
    double x_ = *x;
    double y_ = *y;

    *x = round(x_ * matrix[0][0] + y_ * matrix[0][1]);
    *y = round(x_ * matrix[1][0] + y_ * matrix[1][1]);
}

bool invert(double matrix[2][2]) {
    double a = matrix[0][0];
    double b = matrix[0][1];
    double c = matrix[1][0];
    double d = matrix[1][1];

    double det = a * d - b * c;

    if (det == 0) {
        return false;
    }

    matrix[0][0] = d / det;
    matrix[0][1] = -b / det;
    matrix[1][0] = -c / det;
    matrix[1][1] = a / det;

    return true;
}

int parse(game *g) {
    char line[MAX_LINE];
    int i = 0;

    while (fgets(line, MAX_LINE, stdin) != NULL) {
        char *c = line;
        if (memcmp("Button A:", line, 9) == 0) {
            assert(i < MAX_GAMES);

            c = strchr(c, 'X') + 1;
            g->a.x = atol(c);
            c = strchr(c, 'Y') + 1;
            g->a.y = atol(c);
        } else if (memcmp("Button B:", line, 9) == 0) {
            c = strchr(c, 'X') + 1;
            g->b.x = atol(c);
            c = strchr(c, 'Y') + 1;
            g->b.y = atol(c);
        } else if (memcmp("Prize:", line, 6) == 0) {
            c = strchr(c, 'X') + 2;
            g->prize.x = atol(c) + PRIZE_OFFSET;
            c = strchr(c, 'Y') + 2;
            g->prize.y = atol(c) + PRIZE_OFFSET;

            g++;
            i++;
        }
    }

    return i;
}

long min_tokens(game *g) {
    coord ac = g->a;
    coord bc = g->b;
    coord prize = g->prize;

    double matrix[2][2] = {{ac.x, bc.x}, {ac.y, bc.y}};
    bool inverted = invert(matrix);
    assert(inverted);

    double a = prize.x;
    double b = prize.y;

    transform(matrix, &a, &b);

    if (a > 0 && b > 0 && a * ac.x + b * bc.x == prize.x &&
        a * ac.y + b * bc.y == prize.y) {
        long cost = 3 * a + b;
        return cost;
    }

    return 0;
}

int main(void) {
    game games[MAX_GAMES];
    long result = 0;
    int count = parse(games);

    for (int i = 0; i < count; i++) {
        result += min_tokens(&games[i]);
    }

    printf("result = %ld\n", result);

    return 0;
}
