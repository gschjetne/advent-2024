#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#define MAX_ROBOTS 512
#define WIDTH 101
#define HEIGHT 103
#define COL_COUNT 1

typedef struct {
    int x;
    int y;
} point;

typedef struct {
    point position;
    point velocity;
} robot;

int parse(robot *r) {
    int count = 0;

    while (scanf("p=%d,%d v=%d,%d\n", &r->position.x, &r->position.y,
                 &r->velocity.x, &r->velocity.y) == 4) {
        r++;
        count++;
    }

    return count;
}

void update(robot *r, int steps) {
    r->position.x =
        (WIDTH * steps + r->position.x + r->velocity.x * steps) % WIDTH;
    r->position.y =
        (HEIGHT * steps + r->position.y + r->velocity.y * steps) % HEIGHT;
}

double chi_squared(robot *r, int count) {
    double result = 0, expected = (double)count / (WIDTH * HEIGHT);
    int grid[HEIGHT][WIDTH] = {0};

    for (int i = 0; i < count; i++) {
        point p = r[i].position;
        grid[p.y][p.x]++;
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            result += pow(grid[y][x] - expected, 2) / expected;
        }
    }

    return result;
}

int main(void) {
    robot robots[MAX_ROBOTS];
    robot reference[MAX_ROBOTS];
    int count = parse(robots);
    int steps = 0;
    double score = chi_squared(robots, count);
    int result = 0;

    memcpy(reference, robots, sizeof(robot) * count);

    do {
        steps++;

        for (int i = 0; i < count; i++) {
            update(&robots[i], 1);
        }

        double cs = chi_squared(robots, count);

        if (cs < score) {
            result = steps;
            score = cs;
        }

    } while (memcmp(robots, reference, sizeof(robot) * count) != 0);

    printf("result = %d\n", result);

    return 0;
}
