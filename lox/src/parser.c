#include "lox/parser.h"
#include "lox/expr.h"
#include "stdbool.h"

bool parser_match(Parser* p, const int array_length, TokenType types[]);
bool parser_isfinished(Parser* p);
Token parser_peek(Parser* p);
bool parser_check(Parser* p, TokenType type);
Token parser_previous(Parser* p);

Expr* expression(Parser* p);
Expr* equality(Parser* p);

Parser parser_init(const Token *tokens) {
  Parser parser = {
    .tokens = tokens,
    .current = 0,
  };
  return parser;
}


Token parser_advance(Parser* p) {
  if (!parser_isfinished(p)) 
    p->current++;
  return parser_previous(p);
}

Token parser_previous(Parser* p) {
  return p->tokens[p->current - 1];
}


bool parser_match(Parser* p, const int array_length, TokenType types[]) {
  for (int i=0; i<array_length; i++) {
    if (parser_check(p, types[i])) {
      parser_advance(p);
      return true;
    }
  }

  return false;
}

bool parser_check(Parser* p, TokenType type) {
  if (parser_isfinished(p))
    return false;

  return parser_peek(p).type == type;
}

bool parser_isfinished(Parser* p) {
  return parser_peek(p).type == FILE_EOF;
}

Token parser_peek(Parser* p) {
  return p->tokens[p->current];
}


Expr* expression(Parser* p) {
  return equality(p);
}  

Expr* equality(Parser* p) {

}
