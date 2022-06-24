#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include "lox/token.h"

typedef struct Parser {
  const Token* tokens;
  unsigned int current;
} Parser;

Parser parser_init(const Token* tokens);

#endif 
