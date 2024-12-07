#include "../lib/vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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

    assert(vector_length(&va) == vector_length(&vb));

    size_t length = vector_length(&va);
    int *aa = (int *)va.base;
    int *ab = (int *)vb.base;

    for (size_t i = 0; i < length; i++) {
        int occurrences = 0;
        int this = aa[i];

        for (size_t j = 0; j < length; j++) {
            if (this == ab[j]) {
                occurrences++;
            }
        }

        result += this * occurrences;
    }

    printf("result = %d\n", result);

    vector_destroy(&vb);
    vector_destroy(&va);

    return 0;
}
