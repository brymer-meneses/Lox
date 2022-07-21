#ifndef LOX_OBJECT_H
#define LOX_OBJECT_H

#include "stdbool.h"
#include "tools/fileloc.h"

typedef enum {
  LOX_NIL,
  LOX_BOOLEAN,
  LOX_STRING,
  LOX_NUMBER,
  LOX_CALLABLE,
  LOX_FUNCTION,
} LoxType;

typedef struct LoxObject {
  union {
    double number;
    char* string;
    bool boolean;
    struct {
      unsigned int arity;
    } function;
    struct {
      unsigned int arity;
      struct LoxObject* arguments;
      char* to_string;
    } callable;
  } as;
  FileLoc* fl;
  LoxType type;
} LoxObject;


char* loxobject__to_string(LoxObject* obj);

LoxObject* loxobject__init(LoxType type, char* lexeme, FileLoc* fl);

LoxObject* loxobject__boolean(bool value, FileLoc* fl);
LoxObject* loxobject__string(char* value, FileLoc* fl);
LoxObject* loxobject__number(double value, FileLoc* fl);
LoxObject* loxobject__nil(FileLoc* fl);
LoxObject* loxobject__callable(LoxObject* callee, FileLoc* fl);

char* loxtype__to_string(LoxType type);


void loxobject__free(LoxObject* obj);
bool loxobject__istruthy(LoxObject* obj);
bool loxobject__isequal(LoxObject* obj1, LoxObject* obj2);

#endif // !LOX_OBJECT_H
