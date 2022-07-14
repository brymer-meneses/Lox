#ifndef LOX_OBJECT_H
#define LOX_OBJECT_H

#include "stdbool.h"
#include "tools/fileloc.h"

typedef enum {
  LOX_NIL,
  LOX_BOOLEAN,
  LOX_STRING,
  LOX_NUMBER,
} LoxType;

typedef struct {
  union {
    double number;
    char* string;
    bool boolean;
  } data;
  FileLoc* fl;
  LoxType type;
} LoxObject;


char* loxobject_to_string(LoxObject* obj);

LoxObject* loxobject_init(LoxType type, char* lexeme, FileLoc* fl);

LoxObject* loxobject_boolean(bool value, FileLoc* fl);
LoxObject* loxobject_string(char* value, FileLoc* fl);
LoxObject* loxobject_number(double value, FileLoc* fl);

char* loxtype_to_string(LoxType type);

#endif // !LOX_OBJECT_H


