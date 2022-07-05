#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include "declarations.h"
#include "expr.h"
#include "stmt.h"

typedef struct Parser {
  const Token* tokens;
  const char* source;
  size_t current;
  size_t num_stmts;
} Parser;



void parser_init(const Token* tokens);
Stmt** parser_parse();

#endif 
