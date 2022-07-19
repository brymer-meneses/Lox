#include "lox/environment.h"
#include "lox/lox.h"
#include "lox/object.h"
#include "lox/error.h"
#include "tools/array.h"
#include "tools/hashmap.h"

Environment* environment_init(Environment* enclosing) {
  Environment* env = malloc(1 * sizeof(Environment));
  env->values = hashmap_init();
  env->enclosing = enclosing;
  env->children_environment_array = array_init(sizeof(Environment*));

  if (enclosing != NULL)
    array_append(enclosing->children_environment_array, env);


  return env;
}

void environment_define(Environment* env, char* name, LoxObject* value) {
  hashmap_insert(env->values, name, value);
}

void environment_dump(Environment* env) {
  bool is_emptry = true;
  for (size_t i=0; i<env->values->max_size; i++) {
    const HashmapEntry* entry = env->values->entries[i];
    if (entry != NULL) {
      is_emptry = false;
      printf("[SLOT: %lu] %s -> %s\n", i, entry->key, loxobject_to_string((LoxObject*) entry->value ));
    }
  }

  if (is_emptry) 
    printf("EMPTY ENVIRONMENT\n");
}


LoxObject* environment_get(Environment* env, Token* name) {
  void* result = hashmap_retrieve(env->values, name->lexeme);


  if (result != NULL) return (LoxObject*) result; 

  if (env->enclosing != NULL) return environment_get(env->enclosing, name);

  report(name->fileloc, "Undefined variable: %s", name->lexeme);
  return NULL;
}

void environment_assign(Environment* env, Token* name, LoxObject* value) {
  void* result = hashmap_retrieve(env->values, name->lexeme);

  if (result != NULL) {
    hashmap_insert(env->values, name->lexeme, value);
    return;
  }
  if (env->enclosing != NULL) {
    environment_assign(env->enclosing, name, value);
    return;
  }
  report(name->fileloc, "Undefined variable: %s.", name->lexeme);

}

void environment_free(Environment* env) {
  hashmap_free(env->values);

  for (size_t i=0; i<env->children_environment_array->curr_size; i++) {
    environment_free(env->children_environment_array->elements[i]);
  }

  free(env->children_environment_array);
}
