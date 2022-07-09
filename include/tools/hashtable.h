#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "lox/object.h"
#include "stdio.h"

typedef struct { 
  union {
    TokenType tokentype;
    char* string;
  } value;
  bool isnull;
} HTValue;

typedef struct HTItem {
  char* key;
  HTValue data;
} HTItem;

typedef struct HashTable {
  size_t _capacity;
  size_t _num_items;
  HTItem* items;
} HashTable;

HashTable ht_init();
HTValue ht_retrieve(HashTable* ht, const char* key);
void ht_insert(HashTable* ht, const char* key, HTValue value);


#endif // !HASH_TABLE_H 

