#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 80
#define MAX_OPERANDS 10

enum op_type { ADDITION, MULTIPLICATON, CONCATENATION };

bool inc_op_type(enum op_type *t, int count) {
    for (int i = 0; i < count; i++) {
        switch (t[i]) {
        case ADDITION:
            t[i] = MULTIPLICATON;
            return true;
        case MULTIPLICATON:
            t[i] = CONCATENATION;
            return true;
        case CONCATENATION:
            t[i] = ADDITION;
            break;
        }
    }

    return false;
}

long concatenate(long a, long b) {
    char str[20];
    sprintf(str, "%ld%ld", a, b);
    return atol(str);
}

int main(void) {
    long result = 0;

    char line[MAX_LINE_SIZE];
    while (fgets(line, MAX_LINE_SIZE, stdin)) {

        int operand_count = 0;
        long left, right[MAX_OPERANDS];

        char *t = strtok(line, " ");
        left = atol(t);

        while ((t = strtok(NULL, " ")) != NULL) {
            right[operand_count++] = atol(t);
        }

        enum op_type operators[MAX_OPERANDS];
        int operator_count = operand_count - 1;

        for (int i = 0; i < operator_count; i++) {
            operators[i] = ADDITION;
        }

        do {
            long acc = right[0];

            for (int i = 1; i < operand_count; i++) {
                switch (operators[i - 1]) {
                case ADDITION:
                    acc += right[i];
                    break;
                case MULTIPLICATON:
                    acc *= right[i];
                    break;
                case CONCATENATION:
                    acc = concatenate(acc, right[i]);
                    break;
                }
            }

            if (acc == left) {
                result += left;
                break;
            }
        } while (inc_op_type(operators, operator_count));
    }

    printf("result = %ld\n", result);

    return 0;
}
