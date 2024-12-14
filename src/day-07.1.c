#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 80
#define MAX_OPERANDS 10

int main(void) {
    long result = 0;

    char line[MAX_LINE_SIZE];
    while (fgets(line, MAX_LINE_SIZE, stdin)) {
        int operands = 0;
        long left, right[MAX_OPERANDS];

        char *t = strtok(line, " ");
        left = atol(t);

        while ((t = strtok(NULL, " ")) != NULL) {
            right[operands++] = atol(t);
        }

        unsigned int operator_combinations = 1 << (operands - 1);

        for (unsigned int op = 0; op < operator_combinations; op++) {
            long acc = right[0];
            for (int i = 1; i < operands; i++) {
                if (op >> (i - 1) & 1) {
                    acc *= right[i];
                } else {
                    acc += right[i];
                }
            }
            if (acc == left) {
                result += left;
                break;
            }
        }
    }

    printf("result = %ld\n", result);

    return 0;
}
