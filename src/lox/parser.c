#include "lox/object.h"
#include "stdbool.h"
#include "stdarg.h"
#include "stdio.h"

#include "lox/filelocation.h"
#include "tools/error.h"
#include "lox/parser.h"
#include "lox/expr.h"
#include "lox/token.h"
#include "lox/lox.h"

static bool  isfinished();
static Token peek();
static bool  check(TokenType type);
static Token previous();

static Expr* parse_equality();
static Expr* parse_expression();
static Expr* parse_primary();
static Expr* parse_comparison();
static Expr* parse_term();
static Expr* parse_factor();
static Expr* parse_unary();
static Expr* parse_primary();


void parser_init(const Token *tokens) {
  lox.parser = (Parser) {
    .tokens = tokens,
    .current = 0,
  };
}

Expr* parser_parse() {

  Expr* expr = parse_expression();

  if (lox.had_error) {
    return NULL;
  }

  return expr;
}


static Token advance() {
  if (!isfinished()) 
    lox.parser.current++;
  return previous();
}

static Token previous() {
  return lox.parser.tokens[lox.parser.current - 1];
}


// Similar to parser_check but consumes the token
static bool match(int num_types, ...) {
  va_list token_types;
  va_start(token_types, num_types);

  for (int i=0; i<num_types; i++) {
    TokenType type = va_arg(token_types, TokenType);
    if (check(type)) {
      advance();
      return true;
    }
  }

  va_end(token_types);
  return false;
}

// checks if the current token being scanned is of the "expected" type
static bool check(TokenType expected) {
  if (isfinished())
    return false;

  return peek().type == expected;
}

static bool isfinished() {
  return peek().type == SOURCE_END;
}

static Token peek() {
  return lox.parser.tokens[lox.parser.current];
}

static void synchronize() {
  advance();

  while (!isfinished()) {
    if (previous().type == SEMICOLON) return;

    switch (peek().type) {
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
    advance();
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

static Expr* parse_expression() {
  return parse_equality();
}  

static Expr* parse_equality() {
  Expr* expr = parse_comparison();

  while (match(2, BANG_EQUAL, EQUAL_EQUAL)) {
    Token operator = previous();
    Expr* right = parse_comparison();
    expr = binary(expr, operator, right);
  }

  return expr;
}

static Expr* parse_comparison() {
  Expr* expr = parse_term();
  while (match(4, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    Token operator = previous();
    Expr* right = parse_term();
    expr = binary(expr, operator, right);
  }
  return expr;
}

static Expr* parse_term() {
  Expr* expr = parse_factor();

  while (match(2, MINUS, PLUS)) {
    Token operator = previous();
    Expr* right = parse_factor();
    expr = binary(expr, operator, right);
  }

  return expr;
}

static Expr* parse_factor() {
  Expr* expr = parse_unary();

  while (match(2, SLASH, STAR)) {
    Token operator = previous();
    Expr* right = parse_unary();
    expr = binary(expr, operator, right);
  }
  return expr;
}

static Expr* parse_unary() {
  if (match(2, BANG, MINUS)) {
    Token operator = previous();
    Expr* right = parse_unary();
    return unary(operator, right);
  }

  return parse_primary();
}

static Expr* parse_primary() {
  if (match(1, FALSE)) return literal(encode_bool(false));
  if (match(1, TRUE)) return literal(encode_bool(true));
  if (match(1, NIL)) return literal(LOX_OBJECT_NULL);

  if (match(1, NUMBER)) 
    return literal(previous().literal);
  if (match(1, STRING)) 
    return literal(previous().literal);

  if (match(1, LEFT_PAREN)) {
    Expr* expr = parse_expression();

    if (!check(RIGHT_PAREN)) {
      const Token p = previous();

      FileLoc fl = fileloc_init(p.fileloc.line, p.fileloc.start, p.fileloc.end);

      raise_expected_token_error(")", fl);
    }
    return grouping(expr);
  }

  return NULL;
}

