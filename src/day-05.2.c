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

bool in_order(int *update, int page_count) {
    order_spec *specs = order_specs.base;
    size_t length = vector_length(&order_specs);

    for (int i = 0; i < page_count - 1; i++) {
        for (int j = i + 1; j < page_count; j++) {
            int first = update[i], second = update[j];
            for (int i = 0; i < length; i++) {
                order_spec spec = specs[i];

                if (first == spec.second && second == spec.first) {
                    return false;
                }
            }
        }
    }

    return true;
}

int compare(const void *first, const void *second) {
    order_spec *specs = order_specs.base;
    size_t length = vector_length(&order_specs);
    int f = *((int *)first);
    int s = *((int *)second);

    for (int i = 0; i < length; i++) {
        order_spec spec = specs[i];

        if (f == spec.first && s == spec.second) {
            return -1;
        } else if (f == spec.second && s == spec.first) {
            return 1;
        }
    }

    return 0;
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

    while (fgets(line, MAX_LINE_SIZE, stdin)) {
        int update[MAX_PAGE_COUNT];
        int page_count = 0;
        char *token = strtok(line, ",");
        while (token != NULL) {
            update[page_count++] = atoi(token);
            token = strtok(NULL, ",");
        }

        if (!in_order(update, page_count)) {
            qsort(update, page_count, sizeof(int), compare);
            result += update[page_count / 2];
        }
    }

    printf("result = %d\n", result);

    vector_destroy(&order_specs);
    return 0;
}
