#include "tools/hashtable.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

static void ht_item_free(HTItem* item);
static HTItem ht_item_init(char* key, char* value);
static unsigned long hash(unsigned char *str);

HashTable ht_init() {
  return (HashTable) { 
    ._capacity = INITIAL_CAPACITY,
    ._num_items = 0,
    .items = calloc(INITIAL_CAPACITY, sizeof(HTItem)),
  };
}

char* ht_retrieve(HashTable* ht, char* key) {
  assert(ht != NULL);
  assert(key != NULL);

  unsigned long hashed_key = hash((unsigned char*) key) % ht->_capacity;

  while (ht->items[hashed_key].key != key) {

    if (hashed_key + 1 > ht->_capacity)
      return NULL;

    hashed_key++;
  }

  return ht->items[hashed_key].value;
}

void ht_insert(HashTable* ht, char* key, char* value) {
  assert(ht != NULL);
  assert(key != NULL);
  assert(value != NULL);

  HTItem item = ht_item_init(key, value);

  if (ht->_num_items +1 >= ht->_capacity) {
    ht->_capacity *= 2;
    ht->items = realloc(ht->items, sizeof(HTItem) * ht->_capacity);
  }

  unsigned long hashed_key = hash((unsigned char*) key) % ht->_capacity;

  if (ht->items[hashed_key].value == NULL) {
    ht->items->value = value;
    ht->items->key = key;
  } else {
    while (ht->items[hashed_key].value != NULL) {
      hashed_key++;
    }
  }

  ht->items[hashed_key] = item;
  ht->_num_items++;
}

void ht_free(HashTable* ht) {
  assert(ht != NULL);
  for (size_t i=0; i<ht->_capacity; i++) {
    ht_item_free(&ht->items[i]);
  }
}

static void ht_item_free(HTItem* item) {
  free(item->value);
  free(item->key);
}


static HTItem ht_item_init(char* key, char* value) {
  return (HTItem) {
    .key = key,
    .value = value,
  };
}

// This function was taken from here: https://stackoverflow.com/a/7666577
static unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

