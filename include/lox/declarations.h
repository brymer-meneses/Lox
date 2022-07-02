#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define MAX_INPUT_LIMIT 1024
#define MAX_STRING_LIMIT 2147483647

#define INITIAL_TOKEN_ARRAY_SIZE 8

#include "stdbool.h"
#include "stdlib.h"

typedef enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  SOURCE_END
} TokenType;

typedef struct {
  union {
    double number;
    char* string;
    bool boolean;
  } data;
  TokenType type;
} LoxObject;


typedef struct {
   size_t line;
   size_t start;
   size_t end;
} FileLoc;

typedef struct Token {
  TokenType type;
  size_t line;
  char* lexeme;
  LoxObject literal;
  FileLoc fileloc;
} Token;

typedef struct Scanner {
  // points to the next character
  size_t current;
  // points to the start of a prospective token
  size_t start;
  size_t line;
  const char *source;

  Token* tokens;
  size_t parsed;
  size_t capacity;

  // position of the last line character
  size_t last_line;
} Scanner;

typedef enum ExprType {
  EXPR_BINARY,
  EXPR_UNARY,
  EXPR_LITERAL,
  EXPR_GROUPING,
} ExprType;


typedef struct Expr {
  ExprType type;
  struct Expr *left;
  struct Expr *right;
  LoxObject value;
  Token op;
} Expr;


typedef struct Parser {
  const Token* tokens;
  const char* source;
  unsigned int current;
} Parser;

typedef struct Lox {
  Scanner scanner;
  Parser parser;

  bool had_error;
  bool had_runtime_error;
} Lox;

extern FileLoc FILE_LOC_NULL;
extern Token TOKEN_NULL;
extern LoxObject LOX_OBJECT_NULL;

#endif
