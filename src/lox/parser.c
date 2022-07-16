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
 * assignment     → IDENTIFIER "=" assignment | equality | logic_or ;
 * logic_or       → logic_and ( "or" logic_and )"* ;
 * logic_and      → equality ( "and" eqaulity )"* ;
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

static Expr* or();
static Expr* and();
/*
 * program        → declaration* EOF ;
 * declaration    → varDecl | statement ;
 * statement      → exprStmt | printStmt | block | ifStmt ;
 *
 * varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
 * exprStmt       →  expression ";" ;
 * printStmt      → "print" expression ";" ;
 * ifStmt         → "if" "(" expression ")" statement
 *                   ( "else" statement )? ;
 */

static Stmt* declaration();
static Stmt* statement();
static Stmt* if_statement();
static Stmt* expression_statement();
static Stmt* print_statement();
static Stmt* var_declaration();
static Stmt* while_loop_statement();
static Stmt* for_loop_statement();

static Array* block();

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
  Expr* expr = or();

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

static Expr* or() {
  Expr* expr = and();

  while (match(1, OR)) {
    Token* operator = previous();
    Expr* right = and();
    expr = logical_init(expr, operator, right);
  }

  return expr;
}

static Expr* and() {
  Expr* expr = equality();

  while (match(1, AND)) {
    Token* operator = previous();
    Expr* right = equality();
    expr = logical_init(expr, operator, right);
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
    return literal_init(loxobject_nil(previous()->fileloc));

  if (match(1, STRING))
    return literal_init(loxobject_init(LOX_STRING, previous()->literal->as.string, previous()->fileloc));
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
    return var_init(previous());
  }



  return NULL;
}

static Stmt* statement() {
  if (match(1, FOR))
    return for_loop_statement();
  if (match(1, IF)) 
    return if_statement();
  if (match(1, PRINT)) 
    return print_statement();
  if (match(1, LEFT_BRACE))
    return stmt_block_init(block());
  if (match(1, WHILE))
    return while_loop_statement();

  return expression_statement();
}

static Stmt* if_statement() {
  expect(find_last_occurence(IF), LEFT_PAREN, "Expected '(' after 'if'.");
  Expr* condition = expression();
  expect(condition->fileloc, RIGHT_PAREN, "Expected ')' after condition.");

  Stmt* then_branch = statement();
  Stmt* else_branch = NULL;
  if (match(1, ELSE)) {
    else_branch = statement();
  }
  return stmt_if_init(condition, then_branch, else_branch);
}

static Stmt* for_loop_statement() {
  expect(find_last_occurence(FOR), LEFT_PAREN, "Expected '(' after 'for'.");

  Stmt* initializer = NULL;
  if (match(1, SEMICOLON)) {
    initializer = NULL;
  } else if (match(1, VAR)) {
    initializer = var_declaration();
  } else {
    initializer = expression_statement();
  }

  Expr* condition = NULL;
  if (!check(SEMICOLON)) {
    condition = expression();
  }

  expect(previous()->fileloc, SEMICOLON, "Expect ';' after loop condition.");

  Expr* increment = NULL;
  if (!check(RIGHT_PAREN)) {
    increment = expression();
  }

  expect(previous()->fileloc, RIGHT_PAREN, "Expect ')' after for clauses");

  Stmt* body = statement();
  if (increment != NULL) {
    Array* stmts_array = array_init(sizeof(Stmt*));
    array_append(stmts_array, body);
    array_append(stmts_array, increment);
    body = stmt_block_init(stmts_array);
  }

  if (condition == NULL) {
    condition = literal_init(loxobject_boolean(true, previous()->fileloc));
  }

  body = stmt_while_loop_init(condition, body);

  if (initializer != NULL) {
    Array* stmts_array = array_init(sizeof(Stmt*));
    array_append(stmts_array, initializer);
    array_append(stmts_array, body);
    body = stmt_block_init(stmts_array);
  }

  return body;
}

static Stmt* while_loop_statement() {
  expect(find_last_occurence(WHILE), LEFT_PAREN, "Expected '(' after while.");
  Expr* condition = expression();
  expect(condition->fileloc, RIGHT_PAREN, "Expected '(' after condition.");
  Stmt* body = statement();

  return stmt_while_loop_init(condition, body);
}

static Stmt* expression_statement() {
  Expr* expr = expression();
  expect(expr->fileloc, SEMICOLON, "Expected ';' after value.");
  return stmt_expr_init(expr);
}

static Array* block() {
  Array* statements = array_init(sizeof(Stmt*));

  while (!check(RIGHT_BRACE) && !isfinished()) {
    array_append(statements, declaration());
  }

  expect(previous()->fileloc, RIGHT_BRACE, "Expected '}' after block.");
  return statements;
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

