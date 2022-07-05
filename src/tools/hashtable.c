#include "stdlib.h"
#include "string.h"
#include "assert.h"

#include "tools/hashtable.h"
#define INITIAL_TABLE_CAPACITY 64
#define REALLOC_FACTOR 0.5

static unsigned long hash(const char *str);

HashTable ht_init() {

  HashTable table = {
    ._capacity = INITIAL_TABLE_CAPACITY,
    ._num_items = 0,
    .items = malloc(INITIAL_TABLE_CAPACITY * sizeof(HTItem)),
  };

  return table;
}

HTValue ht_retrieve(HashTable* ht, const char* key) {

  unsigned long index = hash(key) % ht->_capacity;

  while (strcmp(ht->items[index].key, key) != 0) {
   if (index >= ht->_capacity ) 
      return (HTValue) {.isnull = true};
   index++;
  };

  return ht->items[index].data;
}

void ht_insert(HashTable* ht, const char* key, HTValue data) {
  if (ht->_num_items >= REALLOC_FACTOR * ht->_capacity) {
    ht->_capacity *= 2;
    ht->items = realloc(ht->items, ht->_capacity * sizeof(HTItem));
  }

  unsigned long index = hash(key) % ht->_capacity;

  while (ht->items[index].key != NULL ) {
   if (index >= ht->_capacity ) return;
   index++;
  }

  ht->items[index].key   = malloc(strlen(key) * sizeof(char) +1);
  ht->items[index].data = data;
  ht->items[index].data.isnull = false;

  ht->items[index].key   = strdup(key);
  ht->_num_items++;
}

void ht_free(HashTable* ht) {

}

static unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != 0)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
