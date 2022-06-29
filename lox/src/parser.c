#include "lox/parser.h"
#include "lox/expr.h"
#include "lox/token.h"
#include "stdbool.h"
#include "stdarg.h"

bool  parser_isfinished(Parser* p);
Token parser_peek(Parser* p);
bool  parser_check(Parser* p, TokenType type);
Token parser_previous(Parser* p);

Expr* expression(Parser* p);
Expr* equality(Parser* p);

Parser parser_init(const Token *tokens) {
  return (Parser) {
    .tokens = tokens,
    .current = 0,
  };
}


Token parser_advance(Parser* p) {
  if (!parser_isfinished(p)) 
    p->current++;
  return parser_previous(p);
}

Token parser_previous(Parser* p) {
  return p->tokens[p->current - 1];
}


// Similar to parser_check but consumes the token
bool parser_match(Parser* p, int num_types, ...) {
  va_list token_types;
  va_start(token_types, num_types);

  for (int i=0; i<num_types; i++) {
    TokenType type = va_arg(token_types, TokenType);

    if (parser_check(p, type)) {
      parser_advance(p);
      return true;
    }
  }

  va_end(token_types);
  return false;
}

// checks if the current token being scanned is of the "expected" type
bool parser_check(Parser* p, TokenType expected) {
  if (parser_isfinished(p))
    return false;

  return parser_peek(p).type == expected;
}

bool parser_isfinished(Parser* p) {
  return parser_peek(p).type == SOURCE_END;
}

Token parser_peek(Parser* p) {
  return p->tokens[p->current];
}


Expr* expression(Parser* p) {
  return equality(p);
}  

Expr* equality(Parser* p) {

}
