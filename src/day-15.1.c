#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#define SIZE 50

typedef enum { FREE, BOX, WALL } cell_state;

typedef enum { NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3 } direction;

typedef struct cell_s {
    cell_state state;
    struct cell_s *neighbour[4];
} cell;

cell *parse_grid(cell grid[SIZE][SIZE]) {
    int chr;
    cell *robot = NULL;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            chr = getchar();
            cell *this = &grid[y][x];

            this->neighbour[NORTH] = y > 0 ? &grid[y - 1][x] : NULL;
            this->neighbour[SOUTH] = y < SIZE - 1 ? &grid[y + 1][x] : NULL;
            this->neighbour[EAST] = x < SIZE - 1 ? &grid[y][x + 1] : NULL;
            this->neighbour[WEST] = x > 0 ? &grid[y][x - 1] : NULL;

            switch (chr) {
            case '.':
                this->state = FREE;
                break;
            case 'O':
                this->state = BOX;
                break;
            case '#':
                this->state = WALL;
                break;
            case '@':
                this->state = FREE;
                robot = this;
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

cell *push(cell *c, direction d) {
    cell *next = c->neighbour[d];
    assert(next != NULL);

    switch (next->state) {
    case FREE:
        next->state = c->state;
        return next;
    case WALL:
        return c;
    case BOX:
        if (push(next, d) == next) {
            return c;
        } else {
            next->state = c->state;
            return next;
        }
    default:
        assert(false);
    }
}

void run(cell **robot) {
    int chr;
    while (true) {
        chr = getchar();
        switch (chr) {
        case '^':
            *robot = push(*robot, NORTH);
            break;
        case 'v':
            *robot = push(*robot, SOUTH);
            break;
        case '>':
            *robot = push(*robot, EAST);
            break;
        case '<':
            *robot = push(*robot, WEST);
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

int sum_coordinates(cell grid[SIZE][SIZE]) {
    int result = 0;

    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (grid[y][x].state == BOX) {
                result += 100 * y + x;
            }
        }
    }

    return result;
}

int main(void) {
    cell grid[SIZE][SIZE];
    cell *robot = parse_grid(grid);
    run(&robot);
    printf("result = %d\n", sum_coordinates(grid));

    return 0;
}
