#include <assert.h>
#include <stdio.h>
#define WIDTH 140
#define HEIGHT 140
#define UNASSIGNED -1
#define MAX_REGIONS 1024

struct patch {
    char plant;
    char perimeter_sides;
    char perimeter_sides_counted;
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

enum direction { NORTH = 1, SOUTH = 2, EAST = 4, WEST = 8 };

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
            p->perimeter_sides_counted = 0;

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

enum direction assign_region(struct patch *p, char plant, int id,
                             enum direction dir) {
    if (p == NULL) {
        return dir;
    }

    if (p->plant != plant) {
        return dir;
    }

    if (p->id == id) {
        return 0;
    }

    p->id = id;

    p->perimeter_sides |= assign_region(p->north, plant, id, NORTH);
    p->perimeter_sides |= assign_region(p->south, plant, id, SOUTH);
    p->perimeter_sides |= assign_region(p->east, plant, id, EAST);
    p->perimeter_sides |= assign_region(p->west, plant, id, WEST);

    return 0;
}

void assign_regions(struct patch map[HEIGHT][WIDTH]) {
    int id = 0;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            struct patch *p = &map[y][x];

            if (p->id == UNASSIGNED) {
                assign_region(p, p->plant, id++, 0);
            }
        }
    }
}

int find_edges(struct patch *p, enum direction dir, int id) {
    if (p == NULL) {
        return 0;
    }

    if (p->id != id) {
        return 0;
    }

    if (p->perimeter_sides_counted & dir) {
        return 0;
    }

    if (!(p->perimeter_sides & dir)) {
        return 0;
    }

    switch (dir) {
    case NORTH:
    case SOUTH:
        for (struct patch *east = p->east;
             east != NULL && east->id == id && east->perimeter_sides & dir;
             east = east->east) {
            east->perimeter_sides_counted |= dir;
        }
        for (struct patch *west = p->west;
             west != NULL && west->id == id && west->perimeter_sides & dir;
             west = west->east) {
            west->perimeter_sides_counted |= dir;
        }
        break;
    case EAST:
    case WEST:
        for (struct patch *north = p->north;
             north != NULL && north->id == id && north->perimeter_sides & dir;
             north = north->north) {
            north->perimeter_sides_counted |= dir;
        }
        for (struct patch *south = p->south;
             south != NULL && south->id == id && south->perimeter_sides & dir;
             south = south->south) {
            south->perimeter_sides_counted |= dir;
        }
        break;
    }

    p->perimeter_sides_counted |= dir;

    return 1;
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
            int id = p->id;

            assert(id < MAX_REGIONS);
            struct cost *r = &regions[id];

            r->area++;
            r->perimeter += find_edges(p, NORTH, id);
            r->perimeter += find_edges(p, SOUTH, id);
            r->perimeter += find_edges(p, EAST, id);
            r->perimeter += find_edges(p, WEST, id);
        }
    }

    for (int i = 0; i < MAX_REGIONS; i++) {
        result += regions[i].area * regions[i].perimeter;
    }

    printf("result = %ld\n", result);

    return 0;
}
