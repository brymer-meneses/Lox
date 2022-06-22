#ifndef LOX_OBJECT_H
#define LOX_OBJECT_H

#include <stdbool.h>

typedef struct LoxObject {
  union {
    double number;
    char* string;
    bool boolean;
  } value;

  enum {
    LOX_NUMBER,
    LOX_STRING,
    LOX_BOOLEAN,
    LOX_NULL,
  } type;

} LoxObject;

double parse_number(const LoxObject obj);
char* parse_string(const LoxObject obj);
bool parse_bool(const LoxObject obj);

#endif
