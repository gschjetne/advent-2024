#include "../lib/vector.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int is_safe(vector *report) {
    size_t length = vector_length(report);
    int *elements = (int *)report->base;
    int direction = 0;

    for (int i = 1; i < length; i++) {
        int change = elements[i - 1] - elements[i];
        int abs_change = abs(change);

        if ((direction > 0 && change < 0) || (direction < 0 && change > 0)) {
            return 0;
        }

        if (abs_change < 1 || abs_change > 3) {
            return 0;
        }

        direction = change;
    }
    return 1;
}

int read_numbers(vector *numbers) {
    vector_clear(numbers);

    bool has_char = false;
    int c, n = 0;

    while (true) {
        c = getchar();

        if (c == '\n' || c == EOF) {
            if (has_char) {
                vector_push(numbers, &n);
            }

            break;
        } else if (c == ' ') {
            has_char = false;
            vector_push(numbers, &n);
            n = 0;
        } else if (c >= '0' && c <= '9') {
            has_char = true;
            n = (n * 10) + (c - '0');
        } else {
            assert(false);
        }
    }

    return vector_length(numbers);
}

int main(void) {
    vector report;

    vector_init(&report, sizeof(int));

    int result = 0;

    while (read_numbers(&report)) {
        result += is_safe(&report);
    }

    printf("result = %d\n", result);

    vector_destroy(&report);

    return 0;
}
