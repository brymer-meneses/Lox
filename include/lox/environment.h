#ifndef LOX_ENVIRONMENT_H
#define LOX_ENVIRONMENT_H

#include "lox/object.h"
#include "lox/token.h"

#include "tools/hashmap.h"

typedef struct Environment {
  Hashmap* values;
} Environment;

Environment* environment_init();

void environment_define(Environment* env, char* name, LoxObject* value);
LoxObject* environment_get(Environment* env, Token *name);
void environment_assign(Environment* env, Token* name, LoxObject* value);
void environment_dump(Environment* env);


#endif // !LOX_ENVIRONMENT_H
