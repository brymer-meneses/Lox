#ifndef LOX_ENVIRONMENT_H
#define LOX_ENVIRONMENT_H

#include "lox/object.h"
#include "lox/token.h"

#include "tools/array.h"
#include "tools/hashmap.h"

typedef struct Environment {
  Hashmap* values;
  struct Environment* enclosing;
  Array* children_environment_array;
} Environment;

Environment* environment__init(Environment* enclosing);

void environment__define(Environment* env, char* name, LoxObject* value);
void environment__assign(Environment* env, Token* name, LoxObject* value);
void environment__dump(Environment* env);
void environment__free(Environment* env);
LoxObject* environment__get(Environment* env, Token *name);


#endif // !LOX_ENVIRONMENT_H
