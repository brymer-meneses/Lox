#ifndef HASH_MAP_H
#define HASH_MAP_H

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

Hashmap* hashmap__init();
void hashmap__free(Hashmap* hmap);
void hashmap__insert(Hashmap* hmap, char* key, void* value);
void* hashmap__retrieve(Hashmap* hmap, char* key);
void hashmap__print(Hashmap* hmap);

#endif // !HASH_MAP_H 

