#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 56
#define WIDTH 56

struct node {
    int height;
    struct node *north;
    struct node *south;
    struct node *east;
    struct node *west;
};

struct breadcrumb {
    struct node *this;
    struct breadcrumb *prev;
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

bool visited(struct node *n, struct breadcrumb *b) {
    assert(n != NULL);

    if (b == NULL) {
        return false;
    }

    if (b->this == n) {
        return true;
    }

    return visited(n, b->prev);
}

int rate(struct node *n, struct breadcrumb *b) {
    if (n == NULL || visited(n, b)) {
        return 0;
    }

    if (n->height == 9) {
        return 1;
    }

    struct breadcrumb nb = {.this = n, .prev = b};

    int result = 0;
    result += rate(n->north, &nb);
    result += rate(n->south, &nb);
    result += rate(n->east, &nb);
    result += rate(n->west, &nb);

    return result;
}

int main(void) {
    int result = 0;
    struct node map[HEIGHT][WIDTH];
    parse(map);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            struct node *n = &map[y][x];

            if (n->height == 0) {
                result += rate(n, NULL);
                /* debug_print(map); */
            }
        }
    }

    printf("result = %d\n", result);

    return 0;
}
