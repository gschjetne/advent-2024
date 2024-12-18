#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 56
#define WIDTH 56

struct node {
    int height;
    bool visited;
    struct node *north;
    struct node *south;
    struct node *east;
    struct node *west;
};

void parse(struct node map[HEIGHT][WIDTH]) {
    int x = 0, y = 0, input[HEIGHT][WIDTH];

    while (true) {
        int chr = getchar();

        if (chr == '\n') {
            x = 0;
            y++;
        } else if (chr >= '0' && chr <= '9') {
            input[y][x] = chr - '0';
            x++;
        } else {
            break;
        }
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            struct node *n = &map[y][x];
            int h = input[y][x];

            n->height = h;

            if (y > 0 && input[y - 1][x] - h == 1) {
                n->north = &map[y - 1][x];
            } else {
                n->north = NULL;
            }

            if (y < HEIGHT - 1 && input[y + 1][x] - h == 1) {
                n->south = &map[y + 1][x];
            } else {
                n->south = NULL;
            }

            if (x < WIDTH - 1 && input[y][x + 1] - h == 1) {
                n->east = &map[y][x + 1];
            } else {
                n->east = NULL;
            }

            if (x > 0 && input[y][x - 1] - h == 1) {
                n->west = &map[y][x - 1];
            } else {
                n->west = NULL;
            }
        }
    }
}

int rate(struct node *n) {
    if (n == NULL || n->visited) {
        return 0;
    }

    n->visited = true;

    if (n->height == 9) {
        return 1;
    }

    return rate(n->north) + rate(n->south) + rate(n->east) + rate(n->west);
}

void reset_visited(struct node map[HEIGHT][WIDTH]) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            map[y][x].visited = false;
        }
    }
}

int main(void) {
    int result = 0;
    struct node map[HEIGHT][WIDTH];
    parse(map);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            struct node *n = &map[y][x];

            if (n->height == 0) {
                reset_visited(map);
                result += rate(n);
                /* debug_print(map); */
            }
        }
    }

    printf("result = %d\n", result);

    return 0;
}
