#include "stdbool.h"
#include "stdarg.h"

#include "lox/parser.h"
#include "lox/expr.h"

bool  parser_isfinished(Parser* p);
Token parser_peek(Parser* p);
bool  parser_check(Parser* p, TokenType type);
Token parser_previous(Parser* p);

Expr* parse_equality(Parser* p);
Expr* parse_expression(Parser* p);

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

Expr* parse_primary(Parser* p) {
  if (parser_match(p, 1, FALSE)) return literal("false");
  if (parser_match(p, 1, TRUE)) return literal("true");
  if (parser_match(p, 1, NIL)) return literal("null");

  if (parser_match(p, 2, NUMBER, STRING)) {
    return literal(parser_previous(p).lexeme);
  }

  if (parser_match(p, 1, LEFT_PAREN)) {
    Expr* expr = parse_expression(p);
    return grouping(expr);
  }
}

Expr* parse_unary(Parser* p) {
  if (parser_match(p, 2, BANG, MINUS)) {
    Token operator = parser_previous(p);
    Expr* right = parse_unary(p);
    return unary(operator, right);
  }

  return parse_primary(p);
}

Expr* parse_factor(Parser* p) {
  Expr* expr = parse_unary(p);

  while (parser_match(p, 2, SLASH, STAR)) {
    Token operator = parser_previous(p);
    Expr* right = parse_unary(p);
    expr = binary(expr, operator, right);
  }
  return expr;
}

Expr* parse_term(Parser* p) {
  Expr* expr = parse_factor(p);

  while (parser_match(p, 2, MINUS, PLUS)) {
    Token operator = parser_previous(p);
    Expr* right = parse_factor(p);
    expr = binary(expr, operator, right);
  }

  return expr;
}

Expr* parse_comparison(Parser* p) {
  Expr* expr = parse_term(p);
  while (parser_match(p, 4, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    Token operator = parser_previous(p);
    Expr* right = parse_term(p);
    expr = binary(expr, operator, right);
  }
  return expr;
}

Expr* parse_expression(Parser* p) {
  return parse_equality(p);
}  

Expr* parse_equality(Parser* p) {
  Expr* expr = parse_comparison(p);

  while (parser_match(p, 2, BANG_EQUAL, EQUAL_EQUAL)) {
    Token operator = parser_previous(p);
    Expr* right = parse_comparison(p);
    expr = binary(expr, operator, right);
  }

  return expr;
}

