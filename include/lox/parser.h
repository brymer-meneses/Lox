#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include "declarations.h"
#include "expr.h"
#include "stmt.h"

typedef struct Parser {
  const char* source;
  Token** tokens;
  size_t current;
  size_t num_stmts;
  Stmt** stmts;
} Parser;


Parser* parser_init(Token** tokens);
Stmt** parser_parse();

#endif 
