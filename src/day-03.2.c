#include "../lib/vector.h"
#include <stdbool.h>
#include <stdio.h>

vector stack;

void reset() {
    int chr;

    while (vector_pop(&stack, &chr)) {
        ungetc(chr, stdin);
    }
}

bool has_str(char *str) {
    vector_clear(&stack);

    while (*str) {
        int chr = getchar();

        vector_push(&stack, &chr);

        if (chr != *str) {
            reset();
            return false;
        }

        str++;
    }

    printf("%s", str);

    return true;
}

bool has_int(int *out) {
    bool result = false;
    *out = 0;

    while (true) {
        int chr = getchar();

        if (chr < '0' || chr > '9') {
            ungetc(chr, stdin);
            return result;
        }

        *out = (*out * 10) + (chr - '0');
        result = true;
    }

    printf("%d", *out);

    return result;
}

int main(void) {
    vector_init(&stack, sizeof(int));

    int result = 0;
    bool enabled = true;

    while (true) {
        if (has_str("do()")) {
            enabled = true;
        } else if (has_str("don't()")) {
            enabled = false;
        } else if (has_str("mul(")) {
            int i1;
            int i2;

            if (!has_int(&i1)) {
                continue;
            }

            if (!has_str(",")) {
                continue;
            }

            if (!has_int(&i2)) {
                continue;
            }

            if (!has_str(")")) {
                continue;
            }

            if (enabled) {
                result += i1 * i2;
            }
        } else if (getchar() == EOF) {
            break;
        }
    }

    printf("result = %d\n", result);

    vector_destroy(&stack);
    return 0;
}
