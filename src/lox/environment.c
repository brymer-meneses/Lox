#include "lox/environment.h"
#include "lox/lox.h"
#include "lox/object.h"
#include "tools/error.h"
#include "tools/hashmap.h"

void environment_init() {
  lox.environment = (Environment) {
    .values = hashmap_init(),
  };
}

void environment_define(char* name, LoxObject* value) {
  hashmap_insert(lox.environment.values, name, value);
}

void environment_dump() {
  bool is_emptry = true;
  for (size_t i=0; i<lox.environment.values->max_size; i++) {
    const HashmapEntry* entry = lox.environment.values->entries[i];
    if (entry != NULL) {
      is_emptry = false;
      printf("[SLOT: %lu] %s -> %s\n", i, entry->key, loxobject_to_string((LoxObject*) entry->value ));
    }
  }

  if (is_emptry) 
    printf("EMPTY ENVIRONMENT\n");
}


LoxObject* environment_get(Token* name) {
  void* result = hashmap_retrieve(lox.environment.values, name->lexeme);


  if (result != NULL) {
   return (LoxObject*) result;
  }

  report(name->fileloc, "Undefined variable: %s", name->lexeme);
  return NULL;
}

void environment_assign(Token* name, LoxObject* value) {
  void* result = hashmap_retrieve(lox.environment.values, name->lexeme);

  if (result != NULL) {
    hashmap_insert(lox.environment.values, name->lexeme, value);
    return;
  }

  report(name->fileloc, "Undefined variable: %s.", name->lexeme);

}
