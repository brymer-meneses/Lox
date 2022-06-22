#ifndef LITERAL_H
#define LITERAL_H

#include "stdbool.h"

typedef const char* Literal;

typedef enum {
  LITERAL_STRING,
  LITERAL_NUMBER,
  LITERAL_BOOLEAN,
} LiteralType;

double literal_parse_double(Literal literal);
char* literal_parse_string(Literal literal);
bool literal_parse_bool(Literal literal);

#endif // !LITERAL_H
