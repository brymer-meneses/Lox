#ifndef HASH_MAP_H
#define HASH_MAP_H

#include "lox/object.h"
#include "stdio.h"
#include "stdlib.h"

#define HASHMAP_INITIAL_SIZE 256

typedef struct HashmapEntry {
  char* key;
  void* value;
} HashmapEntry;

typedef struct Hashmap {
  HashmapEntry** entries;
  size_t max_size;
  size_t curr_size;
} Hashmap;

Hashmap* hashmap_init();
void hashmap_free(Hashmap* hmap);
void hashmap_insert(Hashmap* hmap, char* key, void* value);
void* hashmap_retrieve(Hashmap* hmap, char* key);
void hashmap_print(Hashmap* hmap);

#endif // !HASH_MAP_H 

