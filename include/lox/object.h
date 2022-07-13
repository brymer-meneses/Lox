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


LoxObject* loxobject_init(LoxType type, char* lexeme, FileLoc* fl);
char* loxobject_to_string(LoxObject* obj);

LoxObject* loxobject_boolean(bool value);
LoxObject* loxobject_string(char* value);
LoxObject* loxobject_number(double value);

#endif // !LOX_OBJECT_H


