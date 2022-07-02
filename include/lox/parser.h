#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include "declarations.h"

void parser_init(const Token* tokens);
Expr* parser_parse();

#endif 
