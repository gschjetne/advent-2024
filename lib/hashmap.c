#include "hashmap.h"
#include "vector.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

void hashmap_init(hashmap *map, size_t keysize, size_t valsize) {
    map->keysize = keysize;
    map->valsize = valsize;

    size_t entry_size = keysize + valsize;

    for (int i = 0; i < HASHMAP_BUCKET_COUNT; i++) {
        vector_init(&map->buckets[i], entry_size);
    }
}

void hashmap_destroy(hashmap *map) {
    for (int i = 0; i < HASHMAP_BUCKET_COUNT; i++) {
        vector_destroy(&map->buckets[i]);
    }
}

unsigned int hashmap_bkdr(void *key, size_t keysize) {
    unsigned int hash = 0;
    unsigned int sneed = 31;
    char *data = key;

    for (size_t i = 0; i < keysize; i++) {
        hash = hash * sneed + data[i];
    }

    return hash % HASHMAP_BUCKET_COUNT;
}

void *hashmap_find(vector *bucket, void *key, size_t keysize, size_t valsize) {
    void *entries = bucket->base;
    void *fill = bucket->fill;

    for (void *entry = entries; entry < fill; entry += (keysize + valsize)) {
        if (memcmp(key, entry, keysize) == 0) {
            return entry;
        }
    }

    return NULL;
}

void hashmap_set(hashmap *map, void *key, void *value) {
    size_t keysize = map->keysize, valsize = map->valsize;
    unsigned int bucket = hashmap_bkdr(key, keysize);
    vector *bucket_v = &map->buckets[bucket];
    void *found = hashmap_find(bucket_v, key, keysize, valsize);

    if (found != NULL) {
        memcpy(found + keysize, value, valsize);
    } else {
        char data[map->keysize + map->valsize];

        memcpy(data, key, keysize);
        memcpy(data + keysize, value, valsize);

        vector_push(&map->buckets[bucket], &data);
    }
}

bool hashmap_get(hashmap *map, void *key, void *value) {
    size_t keysize = map->keysize, valsize = map->valsize;
    unsigned int bucket = hashmap_bkdr(key, keysize);
    vector *bucket_v = &map->buckets[bucket];
    void *found = hashmap_find(bucket_v, key, keysize, valsize);

    if (found != NULL) {
        memcpy(value, found + keysize, valsize);
        return true;
    }

    return false;
}
