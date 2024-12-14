#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define WIDTH 130
#define HEIGHT 130

#define VISITED_VERTICAL 1
#define VISITED_HORIZONTAL 2
#define OBSTRUCTED 3

enum direction { NORTH = 1, EAST = 2, SOUTH = 4, WEST = 8 };

struct cell {
    bool obstructed : 1;
    enum direction visited : 4;
};

void step(enum direction dir, int *x, int *y) {
    switch (dir) {
    case NORTH:
        (*y)--;
        break;
    case EAST:
        (*x)++;
        break;
    case SOUTH:
        (*y)++;
        break;
    case WEST:
        (*x)--;
        break;
    }
}

enum direction turn_right(enum direction dir) {
    switch (dir) {
    case NORTH:
        return EAST;
    case EAST:
        return SOUTH;
    case SOUTH:
        return WEST;
    case WEST:
        return NORTH;
    }

    assert(false);
}

bool in_bounds(int x, int y) {
    return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
}

int solve(struct cell grid[HEIGHT][WIDTH], int x, int y, enum direction dir) {
    while (in_bounds(x, y)) {
        struct cell *c = &grid[y][x];

        if (c->visited & dir) {
            return 1;
        }

        c->visited |= dir;

        int _x = x, _y = y;
        step(dir, &_x, &_y);

        if (!in_bounds(_x, _y)) {
            return 0;
        }

        struct cell *ahead = &grid[_y][_x];

        if (ahead->obstructed) {
            dir = turn_right(dir);
        }

        step(dir, &x, &y);
    }
    return 0;
}

int main(void) {
    int result = 0;
    struct cell grid[HEIGHT][WIDTH];
    memset(grid, 0, sizeof(grid));
    int chr, ix = 0, iy = 0, x = 0, y = 0;
    enum direction dir = NORTH;

    while ((chr = getchar()) != EOF) {
        struct cell *c = &grid[iy][ix];

        switch (chr) {
        case '.':
            ix++;
            break;
        case '#':
            c->obstructed = true;
            ix++;
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

    for (int oy = 0; oy < HEIGHT; oy++) {
        for (int ox = 0; ox < WIDTH; ox++) {
            if (ox == x && oy == y) {
                continue;
            }

            if (grid[oy][ox].obstructed) {
                continue;
            }

            struct cell grid_copy[HEIGHT][WIDTH];
            assert(HEIGHT * WIDTH * sizeof(struct cell) == sizeof(grid));
            memcpy(grid_copy, grid, sizeof(grid));
            grid_copy[oy][ox].obstructed = true;
            result += solve(grid_copy, x, y, dir);
        }
    }

    printf("result = %d\n", result);
    return 0;
}
