#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "lox/object.h"
#include "stdio.h"

#define CAST(X) (char*) X
#define INITIAL_CAPACITY 64

typedef struct HTItem {
  char* key;
  char* value;
} HTItem;

/*
 * For the sake of simplicity, we will just 
 * use the char* type as the value, since 
 * C has no templates, and I don't want to 
 * implement some complicated macro to do so lol.
 *
 * We will do casting to circumvent this issue instead.
 */
typedef struct HashTable {
  size_t _capacity;
  size_t _num_items;
  HTItem* items;
} HashTable;

HashTable ht_init();
char* ht_retrieve(HashTable* ht, char* key);
void ht_insert(HashTable* ht, char* key, char* value);
void ht_free(HashTable* ht);


#endif // !HASH_TABLE_H 

