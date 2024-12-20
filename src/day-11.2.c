#include "../lib/hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 40
#define BLINKS 75

hashmap cache;

typedef struct {
    long stone;
    int blinks;
} cache_key;

long evaluate(long stone, int blinks);

long evaluate_memo(long stone, int blinks) {
    cache_key key = {stone, blinks};
    long result;

    if (hashmap_get(&cache, &key, &result)) {
        return result;
    }

    result = evaluate(stone, blinks);

    hashmap_set(&cache, &key, &result);

    return result;
}

long evaluate(long stone, int blinks) {
    if (blinks <= 0) {
        return 1;
    }

    blinks--;

    if (stone == 0) {
        return evaluate_memo(1, blinks);
    }

    char str[128];

    sprintf(str, "%ld", stone);
    size_t digit_count = strlen(str);

    if (digit_count % 2 == 0) {
        char *right_str = str + digit_count / 2;
        long right = atol(right_str);

        *right_str = '\0';
        long left = atol(str);

        return evaluate_memo(left, blinks) + evaluate_memo(right, blinks);
    }

    return evaluate_memo(stone * 2024, blinks);
}

int main(void) {
    long result = 0;
    char input[MAX_INPUT];
    fgets(input, MAX_INPUT, stdin);
    char *token = strtok(input, " ");

    hashmap_init(&cache, sizeof(cache_key), sizeof(long));

    while (token != NULL) {
        long number = atol(token);
        result += evaluate(number, BLINKS);
        token = strtok(NULL, " ");
    }

    printf("result = %ld\n", result);

    hashmap_destroy(&cache);

    return 0;
}
