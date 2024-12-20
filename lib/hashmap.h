#ifndef HASHMAP_H_
#define HASHMAP_H_

#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>

#define HASHMAP_BUCKET_COUNT 256

typedef struct {
    size_t keysize;
    size_t valsize;
    vector buckets[HASHMAP_BUCKET_COUNT];
} hashmap;

void hashmap_init(hashmap *map, size_t keysize, size_t valsize);

void hashmap_destroy(hashmap *map);

void hashmap_set(hashmap *map, void *key, void *value);

bool hashmap_get(hashmap *map, void *key, void *value);

#endif
