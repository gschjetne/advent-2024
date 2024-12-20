#ifndef VECTOR_H_
#define VECTOR_H_

#include <stdbool.h>
#include <stdlib.h>

#define VECTOR_INTITIAL_ELEMENT_CAPACITY 32

typedef struct {
    void *base;
    void *fill;
    void *end;
    size_t size;
} vector;

void vector_init(vector *vec, size_t size);
void vector_destroy(vector *vec);
void vector_push(vector *vec, const void *element);
bool vector_pop(vector *vec, void *element);
void vector_insert(vector *vec, size_t after_index, void *element);
void vector_clear(vector *vec);
void vector_sort(vector *vec, int (*compare)(const void *, const void *));
void vector_delete(vector *vec, size_t index);
void vector_copy(vector *dst, vector *src);
size_t vector_length(vector *vec);

#endif
