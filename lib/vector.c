#include "vector.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void vector_init(vector *vec, size_t size) {
    size_t initial_capacity_bytes = VECTOR_INTITIAL_ELEMENT_CAPACITY * size;

    void *buffer = malloc(initial_capacity_bytes);

    if (buffer == NULL) {
        perror("vector_init");
        exit(1);
    }

    vec->base = buffer;
    vec->fill = buffer;
    vec->end = buffer + initial_capacity_bytes;
    vec->size = size;

    assert((vec->end - vec->base) % vec->size == 0);
    assert(vec->fill == vec->base);
    assert(vec->size > 0);
    assert(vec->end > vec->base);
}

void vector_destroy(vector *vec) {
    assert(vec->base != NULL);

    free(vec->base);
    memset(vec, 0, sizeof(vector));

    assert(vec->base == NULL);
    assert(vec->fill == NULL);
    assert(vec->end == NULL);
    assert(vec->size == 0);
}

void vector_grow(vector *vec) {
    size_t old_capacity_bytes = vec->end - vec->base;
    size_t new_capacity_bytes = 2 * old_capacity_bytes;

    assert(new_capacity_bytes > old_capacity_bytes);

    size_t fill_offset = vec->fill - vec->base;

    void *new_start = realloc(vec->base, new_capacity_bytes);

    if (new_start == NULL) {
        perror("vector_grow");
        exit(1);
    }

    vec->base = new_start;
    vec->fill = new_start + fill_offset;
    vec->end = new_start + new_capacity_bytes;

    assert((vec->end - vec->base) % vec->size == 0);
    assert((vec->fill - vec->base) % vec->size == 0);
}

void vector_push(vector *vec, const void *element) {
    assert(vec->fill != NULL);
    assert(vec->fill <= vec->end);

    if (vec->fill == vec->end) {
        vector_grow(vec);
    }

    assert(vec->fill + vec->size <= vec->end);

    memcpy(vec->fill, element, vec->size);

    vec->fill += vec->size;
}

bool vector_pop(vector *vec, void *element) {
    if (vec->fill == vec->base) {
        return false;
    }

    void *fill_dec = vec->fill - vec->size;
    memcpy(element, fill_dec, vec->size);

    vec->fill = fill_dec;

    return true;
}

void vector_clear(vector *vec) {
    vec->fill = vec->base;

    assert(vector_length(vec) == 0);
}

void vector_sort(vector *vec, int (*compare)(const void *, const void *)) {
    assert(vec->base != NULL);
    qsort(vec->base, vector_length(vec), vec->size, compare);
}

void vector_delete(vector *vec, size_t index) {
    assert(index < vector_length(vec));

    for (void *element = vec->base + index * vec->size; element < vec->fill;
         element += vec->size) {
        memcpy(element, element + vec->size, vec->size);
    }

    vec->fill -= vec->size;
}

void vector_copy(vector *dest, vector *src) {
    assert(dest->size == src->size);

    size_t src_bytes = src->fill - src->base;

    while (dest->end - dest->base < src_bytes) {
        vector_grow(dest);
    }

    memcpy(dest->base, src->base, src_bytes);

    dest->fill = dest->base + src_bytes;
}

size_t vector_length(vector *vec) {
    return (vec->fill - vec->base) / vec->size;
}
