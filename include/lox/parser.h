#ifndef LOX_PARSER_H
#define LOX_PARSER_H

#include "lox/core.h"

Parser* parser__init(Token** tokens);
Stmt** parser__parse();

#endif 
