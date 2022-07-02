#include "lox/error.h"
#include "stdbool.h"
#include "stdarg.h"
#include "stdio.h"

#include "lox/parser.h"
#include "lox/expr.h"
#include "lox/token.h"
#include "lox/state.h"

bool  parser_isfinished(Parser* p);
Token parser_peek(Parser* p);
bool  parser_check(Parser* p, TokenType type);
Token parser_previous(Parser* p);

Expr* parse_equality(Parser* p);
Expr* parse_expression(Parser* p);
Expr* parse_primary(Parser* p);
Expr* parse_comparison(Parser* p);
Expr* parse_term(Parser* p);
Expr* parse_factor(Parser* p);
Expr* parse_unary(Parser* p);
Expr* parse_primary(Parser* p);

Parser parser_init(const Token *tokens, const char* source) {
  return (Parser) {
    .tokens = tokens,
    .current = 0,
    .source = source,
  };
}

Expr* parser_parse(Parser* p) {

  Expr* expr = parse_expression(p);

  if (LOX_HAD_ERROR) {
    return NULL;
  }

  return expr;
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

void parser_synchronize(Parser* p) {
  parser_advance(p);

  while (!parser_isfinished(p)) {
    if (parser_previous(p).type == SEMICOLON) return;

    switch (parser_peek(p).type) {
      case CLASS:
      case FUN:
      case VAR:
      case FOR:
      case IF:
      case WHILE:
      case PRINT:
      case RETURN:
        return;
      default:
        break;
    }
    parser_advance(p);
  }

}

/* 
 * NOTES 
 * expression     → equality ;
 * equality       → comparison ( ( "!=" | "==" ) comparison )* ;
 * comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
 * term           → factor ( ( "-" | "+" ) factor )* ;
 * factor         → unary ( ( "/" | "*" ) unary )* ;
 * unary          → ( "!" | "-" ) unary | primary ;
 * primary        → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
 */

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

Expr* parse_comparison(Parser* p) {
  Expr* expr = parse_term(p);
  while (parser_match(p, 4, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    Token operator = parser_previous(p);
    Expr* right = parse_term(p);
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

Expr* parse_factor(Parser* p) {
  Expr* expr = parse_unary(p);

  while (parser_match(p, 2, SLASH, STAR)) {
    Token operator = parser_previous(p);
    Expr* right = parse_unary(p);
    expr = binary(expr, operator, right);
  }
  return expr;
}

Expr* parse_unary(Parser* p) {
  if (parser_match(p, 2, BANG, MINUS)) {
    Token operator = parser_previous(p);
    Expr* right = parse_unary(p);
    return unary(operator, right);
  }

  return parse_primary(p);
}

Expr* parse_primary(Parser* p) {
  if (parser_match(p, 1, FALSE)) return literal("false");
  if (parser_match(p, 1, TRUE)) return literal("true");
  if (parser_match(p, 1, NIL)) return literal("null");

  if (parser_match(p, 1, NUMBER)) 
    return literal(parser_previous(p).lexeme);
  if (parser_match(p, 1, STRING)) 
    return literal(token_parse_string(parser_previous(p)));

  if (parser_match(p, 1, LEFT_PAREN)) {
    Expr* expr = parse_expression(p);

    if (!parser_check(p, RIGHT_PAREN)) {
      raise_expected_token_error(p, parser_peek(p));
    }
    return grouping(expr);
  }

  return NULL;
}

