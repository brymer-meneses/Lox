#include "lox/environment.h"
#include "lox/lox.h"
#include "lox/object.h"
#include "lox/error.h"
#include "tools/array.h"
#include "tools/hashmap.h"

Environment* environment__init(Environment* enclosing) {
  Environment* env = malloc(1 * sizeof(Environment));
  env->values = hashmap__init();
  env->enclosing = enclosing;
  env->children_environment_array = array__init(sizeof(Environment*));

  if (enclosing != NULL)
    array__append(enclosing->children_environment_array, env);


  return env;
}

void environment__define(Environment* env, char* name, LoxObject* value) {
  hashmap__insert(env->values, name, value);
}

void environment__dump(Environment* env) {
  bool is_empty = true;
  for (size_t i=0; i<env->values->max_size; i++) {
    const HashmapEntry* entry = env->values->entries[i];
    if (entry != NULL) {
      is_empty = false;
      printf("[SLOT: %lu] %s -> %s\n", i, entry->key, loxobject__to_string((LoxObject*) entry->value ));
    }
  }

  if (is_empty) 
    printf("EMPTY ENVIRONMENT\n");
}


LoxObject* environment__get(Environment* env, Token* name) {
  void* result = hashmap__retrieve(env->values, name->lexeme);

  if (result != NULL) return (LoxObject*) result; 

  if (env->enclosing != NULL) return environment__get(env->enclosing, name);

  report(name->fileloc, "Undefined variable: %s", name->lexeme);
  return NULL;
}

void environment__assign(Environment* env, Token* name, LoxObject* value) {
  void* result = hashmap__retrieve(env->values, name->lexeme);

  if (result != NULL) {
    loxobject__free(result);
    hashmap__insert(env->values, name->lexeme, value);
    return;
  }
  if (env->enclosing != NULL) {
    environment__assign(env->enclosing, name, value);
    return;
  }
  report(name->fileloc, "Undefined variable: %s.", name->lexeme);

}

void environment__free(Environment* env) {
  hashmap__free(env->values);

  for (size_t i=0; i<env->children_environment_array->curr_size; i++) {
    environment__free(env->children_environment_array->elements[i]);
  }

  free(env->children_environment_array);
  free(env);
}
