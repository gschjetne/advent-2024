#include "../lib/vector.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_SIZE 80
#define MAX_PAGE_COUNT 40

typedef struct {
    int first;
    int second;
} order_spec;

vector order_specs;

bool in_order(int first, int second) {
    order_spec *specs = order_specs.base;
    size_t length = vector_length(&order_specs);

    for (int i = 0; i < length; i++) {
        order_spec spec = specs[i];

        if (first == spec.second && second == spec.first) {
            return false;
        }
    }

    return true;
}

int main(void) {
    int result = 0;

    char line[MAX_LINE_SIZE];

    vector_init(&order_specs, sizeof(order_spec));

    order_spec spec;
    while (true) {
        fgets(line, MAX_LINE_SIZE, stdin);

        if (line[1] == 0) {
            break;
        }

        spec.first = atoi(strtok(line, "|"));
        spec.second = atoi(strtok(NULL, "|"));
        vector_push(&order_specs, &spec);
    }

    char linc[MAX_LINE_SIZE];
    while (fgets(line, MAX_LINE_SIZE, stdin)) {
        strcpy(linc, line);
        int update[MAX_PAGE_COUNT];
        int page_count = 0;
        char *token = strtok(line, ",");
        while (token != NULL) {
            update[page_count++] = atoi(token);
            token = strtok(NULL, ",");
        }

        for (int i = 0; i < page_count - 1; i++) {
            for (int j = i + 1; j < page_count; j++) {
                if (!in_order(update[i], update[j])) {
                    goto next_line;
                }
            }
        }

        result += update[page_count / 2];

    next_line:
        continue;
    }

    printf("result = %d\n", result);

    vector_destroy(&order_specs);
    return 0;
}
