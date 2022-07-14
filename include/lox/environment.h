#ifndef LOX_ENVIRONMENT_H
#define LOX_ENVIRONMENT_H

#include "lox/object.h"
#include "lox/token.h"

#include "tools/hashmap.h"

typedef struct Environment {
  Hashmap* values;
} Environment;

void environment_init();

void environment_define(char* name, LoxObject* value);
LoxObject* environment_get(Token *name);
void environment_assign(Token* name, LoxObject* value);
void environment_dump();


#endif // !LOX_ENVIRONMENT_H
