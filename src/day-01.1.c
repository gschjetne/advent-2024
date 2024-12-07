#include "../lib/vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int int_compare(const void *_a, const void *_b) {
    int a = *((int *)_a);
    int b = *((int *)_b);

    return (a > b) - (a < b);
}

int main(void) {
    int a, b;

    int result = 0;

    vector va, vb;

    vector_init(&va, sizeof(int));
    vector_init(&vb, sizeof(int));

    while (scanf("%d   %d\n", &a, &b) != EOF) {
        vector_push(&va, &a);
        vector_push(&vb, &b);
    }

    vector_sort(&va, int_compare);
    vector_sort(&vb, int_compare);

    assert(vector_length(&va) == vector_length(&vb));

    size_t length = vector_length(&va);

    for (size_t i = 0; i < length; i++) {
        int a = ((int *)va.base)[i];
        int b = ((int *)vb.base)[i];

        result += abs(a - b);
    }

    printf("result = %d\n", result);

    vector_destroy(&vb);
    vector_destroy(&va);

    return 0;
}
