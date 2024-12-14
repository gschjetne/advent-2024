#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 130
#define HEIGHT 130

#define VISITED 1
#define OBSTRUCTED 2

bool in_bounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

int main(void) {
    int result = 0;
    char grid[HEIGHT][WIDTH];
    int chr, ix = 0, iy = 0, x = 0, y = 0, dx = 0, dy = -1;

    while ((chr = getchar()) != EOF) {
        switch (chr) {
        case '.':
            grid[iy][ix++] = 0;
            break;
        case '#':
            grid[iy][ix++] = OBSTRUCTED;
            break;
        case '^':
            x = ix;
            y = iy;
            ix++;
            break;
        case '\n':
            ix = 0;
            iy++;
            break;
        default:
            assert(false);
        }
    }

    while (true) {
        grid[y][x] = VISITED;

        int _x = x + dx, _y = y + dy;

        if (!in_bounds(_x, _y)) {
            break;
        }

        char ahead = grid[_y][_x];

        if (ahead & OBSTRUCTED) {
            if (dx == 0 && dy == -1) {
                dx = 1;
                dy = 0;
            } else if (dx == 1 && dy == 0) {
                dx = 0;
                dy = 1;
            } else if (dx == 0 && dy == 1) {
                dx = -1;
                dy = 0;
            } else if (dx == -1 && dy == 0) {
                dx = 0;
                dy = -1;
            } else {
                assert(false);
            }
        }

        x += dx;
        y += dy;
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            result += grid[i][j] & VISITED;
        }
    }

    printf("result = %d\n", result);
    return 0;
}
