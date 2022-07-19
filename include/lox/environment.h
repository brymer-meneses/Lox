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

Environment* environment_init(Environment* enclosing);

void environment_define(Environment* env, char* name, LoxObject* value);
LoxObject* environment_get(Environment* env, Token *name);
void environment_assign(Environment* env, Token* name, LoxObject* value);
void environment_dump(Environment* env);
void environment_free(Environment* env);


#endif // !LOX_ENVIRONMENT_H
