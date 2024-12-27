#include <stdio.h>
#define MAX_ROBOTS 512
#define WIDTH 101
#define HEIGHT 103

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

int main(void) {
    robot robots[MAX_ROBOTS];
    int count = parse(robots);
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;

    for (int i = 0; i < count; i++) {
        robot *r = &robots[i];
        update(r, 100);

        point p = r->position;

        if (p.y < HEIGHT / 2) {
            if (p.x < WIDTH / 2) {
                q1++;
            } else if (p.x > WIDTH / 2) {
                q2++;
            }
        } else if (p.y > HEIGHT / 2) {
            if (p.x < WIDTH / 2) {
                q3++;
            } else if (p.x > WIDTH / 2) {
                q4++;
            }
        }
    }

    printf("result = %d\n", q1 * q2 * q3 * q4);

    return 0;
}
