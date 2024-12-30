#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#define HEIGHT 50
#define WIDTH (HEIGHT * 2)

typedef enum { FREE, BOX_EAST, BOX_WEST, WALL } cell_state;

typedef enum { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3 } direction;

typedef struct cell_s {
    cell_state state;
    struct cell_s *neighbour[4];
} cell;

cell *parse_grid(cell grid[HEIGHT][WIDTH]) {
    int chr;
    cell *robot = NULL;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x += 2) {
            chr = getchar();
            cell *west = &grid[y][x];
            cell *east = &grid[y][x + 1];

            west->neighbour[NORTH] = y > 0 ? &grid[y - 1][x] : NULL;
            east->neighbour[NORTH] = y > 0 ? &grid[y - 1][x + 1] : NULL;

            west->neighbour[SOUTH] = y < HEIGHT - 1 ? &grid[y + 1][x] : NULL;
            east->neighbour[SOUTH] =
                y < HEIGHT - 1 ? &grid[y + 1][x + 1] : NULL;

            west->neighbour[EAST] = east;
            east->neighbour[EAST] = x > 0 ? &grid[y][x + 2] : NULL;

            west->neighbour[WEST] = x < WIDTH - 1 ? &grid[y][x - 1] : NULL;
            east->neighbour[WEST] = west;

            switch (chr) {
            case '.':
                west->state = FREE;
                east->state = FREE;
                break;
            case 'O':
                west->state = BOX_WEST;
                east->state = BOX_EAST;
                break;
            case '#':
                west->state = WALL;
                east->state = WALL;
                break;
            case '@':
                west->state = FREE;
                east->state = FREE;
                robot = west;
                break;
            default:
                assert(false);
            }
        }
        chr = getchar();
        assert(chr == '\n');
    }

    assert(robot != NULL);
    return robot;
}

bool can_push(cell *c, direction d) {
    assert(c != NULL);

    cell_state state = c->state;

    if (state == WALL) {
        return false;
    }

    if (state == FREE) {
        return true;
    }

    if (state == BOX_EAST || BOX_WEST) {
        if (d == EAST || d == WEST) {
            return can_push(c->neighbour[d], d);
        }

        cell *other_half = c->neighbour[state == BOX_EAST ? WEST : EAST];

        return can_push(c->neighbour[d], d) &&
               can_push(other_half->neighbour[d], d);
    }

    assert(false);
}

void do_push(cell *c, cell_state next_state, bool push_other, direction d) {
    assert(c != NULL);

    cell_state state = c->state;

    assert(state != WALL);

    if (state == FREE) {
        c->state = next_state;
        return;
    }

    if (push_other && (d == NORTH || d == SOUTH) &&
        (state == BOX_EAST || state == BOX_WEST)) {
        cell *other_half = c->neighbour[state == BOX_EAST ? WEST : EAST];
        do_push(other_half, FREE, false, d);
    }

    do_push(c->neighbour[d], state, true, d);
    c->state = next_state;
}

cell *push(cell *c, cell_state next_state, direction d) {
    assert(c->state == FREE);
    cell *next = c->neighbour[d];
    if (can_push(next, d)) {
        do_push(next, FREE, true, d);
        return c->neighbour[d];
    }

    return c;
}

void run(cell **robot) {
    int chr;

    while (true) {
        chr = getchar();
        switch (chr) {
        case '^':
            *robot = push(*robot, FREE, NORTH);
            break;
        case 'v':
            *robot = push(*robot, FREE, SOUTH);
            break;
        case '>':
            *robot = push(*robot, FREE, EAST);
            break;
        case '<':
            *robot = push(*robot, FREE, WEST);
            break;
        case '\n':
            break;
        case EOF:
            return;
        default:
            assert(false);
        }
    }
}

int sum_coordinates(cell grid[HEIGHT][WIDTH]) {
    int result = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (grid[y][x].state == BOX_WEST) {
                result += 100 * y + x;
            }
        }
    }

    return result;
}

int main(void) {
    cell grid[HEIGHT][WIDTH];
    cell *robot = parse_grid(grid);
    run(&robot);
    printf("result = %d\n", sum_coordinates(grid));

    return 0;
}
