#include "stdbool.h"
#include "stdarg.h"
#include "stdio.h"
#include "assert.h"

#include "tools/array.h"
#include "tools/error.h"
#include "tools/fileloc.h"
#include "tools/utils.h"

#include "lox/declarations.h"
#include "lox/object.h"
#include "lox/parser.h"
#include "lox/expr.h"
#include "lox/token.h"
#include "lox/lox.h"
#include "lox/stmt.h"

/*
 * expression     → assignment ;
 * assignment     → IDENTIFIER "=" assignment | equality ;
 * equality       → comparison ( ( "!=" | "==" ) comparison )* ;
 * comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
 * term           → factor ( ( "-" | "+" ) factor )* ;
 * factor         → unary ( ( "/" | "*" ) unary )* ;
 * unary          → ( "!" | "-" ) unary | primary ;
 * primary        → NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")" ;
 */

static Expr* assignment();
static Expr* equality();
static Expr* expression();
static Expr* primary();
static Expr* comparison();
static Expr* term();
static Expr* factor();
static Expr* unary();
static Expr* primary();

/*
 * program        → declaration* EOF ;
 * declaration    → varDecl | statement ;
 * statement      → exprStmt | printStmt ;
 *
 * varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
 * exprStmt       →  expression ";" ;
 * printStmt      → "print" expression ";" ;
 */

static Stmt* declaration();
static Stmt* expression_statement();
static Stmt* print_statement();
static Stmt* statement();
static Stmt* var_declaration();

// Utility functions
static bool  isfinished();
static bool  check(TokenType type);
static Token* expect(FileLoc* fl, TokenType type, const char* message);
static Token* peek();
static Token* previous();

static Parser* parser;

Parser* parser_init(Token** tokens) {
  parser = malloc(1 * sizeof(Parser));
  parser->tokens = tokens;
  parser->current = 0;
  return parser;
}

Stmt** parser_parse() {
  assert(parser != NULL);

  Array* arr = array_init(sizeof(Stmt*));

  while (!isfinished()) {
    array_append(arr, declaration());
  }

  parser->stmts = (Stmt**) arr->elements;
  parser->num_stmts = arr->curr_size;

  free(arr);
  return parser->stmts;
}


static Token* advance() {
  if (!isfinished())
    parser->current++;
  return previous();
}

static Token* previous() {
  assert(parser->current != 0);
  return parser->tokens[parser->current - 1];
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

  return peek()->type == expected;
}

static bool isfinished() {
  return peek()->type == SOURCE_END;
}

static Token* peek() {
  return parser->tokens[parser->current];
}

static void synchronize() {
  advance();

  while (!isfinished()) {
    if (previous()->type == SEMICOLON) return;

    switch (peek()->type) {
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

static Token* expect(FileLoc* fl, TokenType type, const char* message) {
  assert(fl != NULL);
  if (check(type)) {
    return advance();
  }

  report(fl, message);
  return NULL;
}

static FileLoc* find_last_occurence(TokenType type) {

  size_t i = parser->current;
  while (i != 0) {
    if (parser->tokens[i]->type == type) {
      return parser->tokens[i]->fileloc;
    }
    i--;
  }

  return parser->tokens[0]->fileloc;
}

static Expr* expression() {
  return assignment();
} 

static Expr* assignment() {
  Expr* expr = equality();

  if (match(1, EQUAL)) {
    Token* equals = previous();
    Expr* value = assignment();

    if (expr->type == EXPR_VAR) {
      Token* name = expr->as.var.name;
      return assign_init(name, value);
    }

    report(equals->fileloc, "Invalid assignment target.");
  }
 return expr;
}

static Stmt* declaration() {
  // Stmt* result = NULL;

  if (match(1, VAR)) 
    return var_declaration();

  return statement();

  // if (lox.had_error) 
  //   synchronize();
  //
  // return result;
}

static Expr* equality() {
  Expr* expr = comparison();

  while (match(2, BANG_EQUAL, EQUAL_EQUAL)) {
    Token* operator = previous();
    Expr* right = comparison();

    if (expr == NULL) {
      report(operator->fileloc, "Expected expression before this.");
      break;
    }

    if (right == NULL) {
      report(operator->fileloc, "Expected expression before this.");
      break;
    }
    expr = binary_init(expr, operator, right);
  }

  return expr;
}

static Expr* comparison() {
  Expr* expr = term();
  while (match(4, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    Token* operator = previous();
    Expr* right = term();

    if (expr == NULL) {
      report(operator->fileloc, "Expected expression before this.");
      break;
    }

    if (right == NULL) {
      report(operator->fileloc, "Expected expression before this.");
      break;
    }
    expr = binary_init(expr, operator, right);
  }
  return expr;
}

static Expr* term() {
  Expr* expr = factor();

  while (match(2, MINUS, PLUS)) {
    Token* operator = previous();
    Expr* right = factor();
    if (right == NULL) {
      report(operator->fileloc, "Expected expression before this.");
      break;
    }
    if (expr == NULL) {
      report(operator->fileloc, "Expected expression before this.");
      break;
    }
    expr = binary_init(expr, operator, right);
  }

  return expr;
}

static Expr* factor() {
  Expr* expr = unary();

  while (match(3, SLASH, STAR, POW)) {
    Token* operator = previous();
    Expr* right = unary();
    if (right == NULL) {
      report(operator->fileloc, "Expected expression before this.");
      break;
    }
    if (expr == NULL) {
      report(operator->fileloc, "Expected expression before this.");
      break;
    }
    expr = binary_init(expr, operator, right);
  }
  return expr;
}

static Expr* unary() {
  if (match(2, BANG, MINUS)) {
    Token* operator = previous();
    Expr* right = unary();
    return unary_init(operator, right);
  }

  return primary();
}

static Expr* primary() {
  if (match(2, FALSE, TRUE)) 
    return literal_init(loxobject_init(LOX_BOOLEAN, previous()->lexeme, previous()->fileloc));

  if (match(1, NIL))  
    return literal_init(loxobject_init(LOX_NIL, "NIL", previous()->fileloc));

  if (match(1, STRING))
    return literal_init(loxobject_init(LOX_STRING, previous()->literal->data.string, previous()->fileloc));
  if (match(1, NUMBER))
    return literal_init(loxobject_init(LOX_NUMBER, previous()->lexeme, previous()->fileloc));

  if (match(1, LEFT_PAREN)) {
    Expr* expr = expression();
    expect(find_last_occurence(LEFT_PAREN), RIGHT_PAREN, "Expected matching ) of this token.");

    if (expr) {
      return grouping_init(expr);
    } 
  }

  if (match(1, IDENTIFIER)) {
    return vardecl_init(previous());
  }


  return NULL;
}

static Stmt* statement() {
  if (match(1, PRINT)) 
    return print_statement();
  return expression_statement();
}

static Stmt* expression_statement() {
  Expr* expr = expression();
  expect(expr->fileloc, SEMICOLON, "Expected ';' after value.");
  return stmt_expr_init(expr);
}

static Stmt* print_statement() {
  Expr* value = expression();
  expect(find_last_occurence(PRINT), SEMICOLON, "Expected ';' after value.");
  return stmt_print_init(value);
}

static Stmt* var_declaration() {
  Token* name = expect(find_last_occurence(VAR), IDENTIFIER, "Expect variable name.");

  Expr* initializer = NULL;

  if (match(1, EQUAL)) {
    initializer = expression();
  }

  expect(name->fileloc, SEMICOLON, "Expect ';' after variable declaration.");

  return stmt_vardecl_init(name, initializer);
}
