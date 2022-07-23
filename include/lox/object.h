#ifndef LOX_OBJECT_H
#define LOX_OBJECT_H

#include "stdbool.h"
#include "lox/core.h"



char* loxobject__to_string(LoxObject* obj);

LoxObject* loxobject__init(LoxType type, char* lexeme, FileLoc* fl);

LoxObject* loxobject__boolean_init(bool value, FileLoc* fl);
LoxObject* loxobject__string_init(char* value, FileLoc* fl);
LoxObject* loxobject__number_init(double value, FileLoc* fl);
LoxObject* loxobject__nil_init(FileLoc* fl);
LoxObject* loxobject__callable_init(char* to_string, unsigned int arity, LoxObject** args, FileLoc* fl);

char* loxtype__to_string(LoxType type);


void loxobject__free(LoxObject* obj);
bool loxobject__istruthy(LoxObject* obj);
bool loxobject__isequal(LoxObject* obj1, LoxObject* obj2);

#endif // !LOX_OBJECT_H
