#include "../lib/vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 40
#define BLINKS 25

void parse(vector *stones) {
    char input[MAX_INPUT];
    fgets(input, MAX_INPUT, stdin);
    char *token = strtok(input, " ");
    while (token != NULL) {
        long number = atol(token);
        vector_push(stones, &number);
        token = strtok(NULL, " ");
    }
}

int main(void) {
    vector stones;
    vector_init(&stones, sizeof(long));

    parse(&stones);

    long *s = stones.base;

    for (int blink = 0; blink < BLINKS; blink++) {
        for (size_t i = 0; i < vector_length(&stones); i++) {
            long number = s[i];
            char str[128];

            if (number == 0) {
                s[i] = 1;
                continue;
            }

            sprintf(str, "%ld", number);
            size_t digit_count = strlen(str);

            if (digit_count % 2 == 0) {
                char *right_str = str + digit_count / 2;
                long right = atol(right_str);

                vector_insert(&stones, i, &right);

                // inserting may realloc, which may move the backing array
                s = stones.base;

                *right_str = '\0';
                s[i] = atol(str);
                i++;
                continue;
            }

            s[i] = number * 2024;
        }
    }

    printf("result = %ld\n", vector_length(&stones));

    vector_destroy(&stones);
    return 0;
}
