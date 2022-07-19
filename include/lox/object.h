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
  } as;
  FileLoc* fl;
  LoxType type;
} LoxObject;


char* loxobject_to_string(LoxObject* obj);

LoxObject* loxobject_init(LoxType type, char* lexeme, FileLoc* fl);

LoxObject* loxobject_boolean(bool value, FileLoc* fl);
LoxObject* loxobject_string(char* value, FileLoc* fl);
LoxObject* loxobject_number(double value, FileLoc* fl);
LoxObject* loxobject_nil(FileLoc* fl);

char* loxtype_to_string(LoxType type);

void loxobject_free(LoxObject* obj);
bool loxobject_istruthy(LoxObject* obj);

bool loxobject_isequal(LoxObject* obj1, LoxObject* obj2);

#endif // !LOX_OBJECT_H
