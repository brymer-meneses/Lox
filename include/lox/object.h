#ifndef LOX_OBJECT_H

#define LOX_OBJECT_H

#include "stdbool.h"
#include "declarations.h"

LoxObject encode_bool(bool value);
LoxObject encode_string(const char* value);
LoxObject encode_double(double value);

char* loxobject_to_string(LoxObject obj);

#endif // !LOX_OBJECT_H


