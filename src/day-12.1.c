#include <assert.h>
#include <stdio.h>
#define WIDTH 140
#define HEIGHT 140
#define UNASSIGNED -1
#define MAX_REGIONS 1024

struct patch {
    char plant;
    char perimeter_sides;
    int id;
    struct patch *north;
    struct patch *south;
    struct patch *east;
    struct patch *west;
};

struct cost {
    int area;
    int perimeter;
};

void parse(struct patch map[HEIGHT][WIDTH]) {
    int chr, x = 0, y = 0;

    while ((chr = getchar()) != EOF) {
        if (chr == '\n') {
            x = 0;
            y++;
        } else {
            struct patch *p = &map[y][x];

            p->plant = chr;
            p->id = UNASSIGNED;
            p->perimeter_sides = 0;

            if (y > 0) {
                p->north = &map[y - 1][x];
            } else {
                p->north = NULL;
            }

            if (y < HEIGHT - 1) {
                p->south = &map[y + 1][x];
            } else {
                p->south = NULL;
            }

            if (x < WIDTH - 1) {
                p->east = &map[y][x + 1];
            } else {
                p->east = NULL;
            }

            if (x > 0) {
                p->west = &map[y][x - 1];
            } else {
                p->west = NULL;
            }

            x++;
        }
    }
}

char assign_region(struct patch *p, char plant, int id) {
    if (p == NULL) {
        return 1;
    }

    if (p->plant != plant) {
        return 1;
    }

    if (p->id == id) {
        return 0;
    }

    p->id = id;

    p->perimeter_sides += assign_region(p->north, plant, id);
    p->perimeter_sides += assign_region(p->south, plant, id);
    p->perimeter_sides += assign_region(p->east, plant, id);
    p->perimeter_sides += assign_region(p->west, plant, id);

    return 0;
}

void assign_regions(struct patch map[HEIGHT][WIDTH]) {
    int id = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            struct patch *p = &map[y][x];

            if (p->id == UNASSIGNED) {
                assign_region(p, p->plant, id++);
            }
        }
    }
}

int main(void) {
    struct patch map[HEIGHT][WIDTH];
    struct cost regions[MAX_REGIONS] = {0};
    long result = 0;

    parse(map);
    assign_regions(map);

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            struct patch *p = &map[y][x];

            assert(p->id < MAX_REGIONS);
            struct cost *r = &regions[p->id];

            r->area++;
            r->perimeter += p->perimeter_sides;
        }
    }

    for (int i = 0; i < MAX_REGIONS; i++) {
        result += regions[i].area * regions[i].perimeter;
    }

    printf("result = %ld\n", result);

    return 0;
}
