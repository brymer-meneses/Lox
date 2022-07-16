#include "tools/hashmap.h"
#include "lox/object.h"
#include "lox/token.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

static unsigned long hash(const char *str);
static HashmapEntry* hashmap_entry(char *key, void* value);

Hashmap* hashmap_init() {
  Hashmap* hmap = malloc(1 * sizeof(Hashmap));
  hmap->curr_size = 0;
  hmap->max_size = HASHMAP_INITIAL_SIZE;
  hmap->entries = calloc(HASHMAP_INITIAL_SIZE, sizeof(HashmapEntry*));

  for (size_t i=0; i<HASHMAP_INITIAL_SIZE; i++) {
    hmap->entries[i] = NULL;
  }

  return hmap;
}

void hashmap_free(Hashmap* hmap) {
  for (size_t i=0; i<hmap->max_size; i++) {

    if (hmap->entries[i] != NULL) {
      free(hmap->entries[i]->value);
      free(hmap->entries[i]->key);
    };

    free(hmap->entries[i]);
  }

  free(hmap->entries);
  free(hmap);
};

void hashmap_print(Hashmap *hmap) {

  for (size_t i=0; i<hmap->max_size; i++) {
    HashmapEntry* entry = hmap->entries[i];

    if (entry != NULL) {
      printf("[SLOT: %lu]: FILLED\n",  i);
    }

  }

}

void hashmap_insert(Hashmap* hmap, char* key,  void* value) {
  unsigned long slot = hash(key) % hmap->max_size;

  if (hmap->curr_size + 1 >= 0.3 * hmap->max_size) {
    hmap->max_size *= 2;
    hmap->entries = realloc(hmap->entries, hmap->max_size * sizeof(HashmapEntry*));
  }

  HashmapEntry* entry = hmap->entries[slot];

  if (entry == NULL) {
    hmap->entries[slot] = hashmap_entry(key, value);
    hmap->curr_size++;
    return;
  }

  while ((entry = hmap->entries[slot]) != NULL) {

    // replace value if a match is found
    if (strcmp(entry->key, key) == 0) {
      hmap->entries[slot]->value = value;
      return;
    }

    slot = (slot + 1) % hmap->max_size;
  }

  hmap->curr_size++;
  hmap->entries[slot] = hashmap_entry(key, value);
};

void* hashmap_retrieve(Hashmap* hmap, char* key) {
  unsigned long slot = hash(key) % hmap->max_size;

  HashmapEntry* entry = hmap->entries[slot];

  if (entry == NULL)  
    return NULL;

  while ((entry = hmap->entries[slot]) != NULL) {

    if (strcmp(entry->key, key) == 0) {
      return entry->value;
    }

    slot = (slot + 1) % hmap->max_size;
  }

  return NULL;
};

static HashmapEntry* hashmap_entry(char* key, void* value) {
  HashmapEntry* entry = malloc(1 * sizeof(HashmapEntry));
  entry->key = malloc(strlen(key) + 1);
  entry->value = value;
  strcpy(entry->key, key);
  return entry;
}


static unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
