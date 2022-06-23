#ifndef LITERAL_H
#define LITERAL_H

#include "stdbool.h"
#include "types.h"

typedef const char* Literal;


double literal_parse_double(Literal literal);
bool literal_parse_bool(Literal literal);

char* literal_to_string(Literal literal);
LoxType literal_get_type(Literal literal);

#endif // !LITERAL_H
