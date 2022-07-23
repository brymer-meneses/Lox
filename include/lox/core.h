#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define MAX_INPUT_LIMIT 1024
#define MAX_STRING_LIMIT 2147483647

#define INITIAL_TOKEN_ARRAY_SIZE 8

#include "stdbool.h"
#include "stdlib.h"

#include "tools/hashmap.h"
#include "tools/array.h"

typedef struct Token Token;
typedef struct LoxObject LoxObject;
typedef struct Stmt Stmt;
typedef struct FileLoc FileLoc;

typedef enum TokenType {
  // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR, POW,

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

typedef struct Token {
  TokenType type;
  char* lexeme;
  LoxObject* literal;
  FileLoc* fileloc;
} Token;

typedef enum {
  LOX_NIL,
  LOX_BOOLEAN,
  LOX_STRING,
  LOX_NUMBER,
  LOX_CALLABLE,
  LOX_FUNCTION,
} LoxType;

typedef struct LoxObject {
  union {
    double number;
    char* string;
    bool boolean;
    struct {
      Stmt* declaration;
    } function;
    struct {
      unsigned int arity;
      struct LoxObject** arguments;
      char* to_string;
    } callable;
  } as;
  FileLoc* fl;
  LoxType type;
} LoxObject;

typedef struct Environment {
  Hashmap* values;
  struct Environment* enclosing;
  Array* children_environment_array;
} Environment;


typedef enum ExprType {
  EXPR_BINARY,
  EXPR_LOGICAL,
  EXPR_UNARY,
  EXPR_LITERAL,
  EXPR_GROUPING,
  EXPR_VAR,
  EXPR_ASSIGN,
  EXPR_CALL,
} ExprType;


typedef struct Expr {
  union {
    struct {
      struct Expr* left;
      struct Expr* right;
      Token* op;
    } binary;

    struct {
      struct Expr* left;
      struct Expr* right;
      Token* op;
    } logical;

    struct {
      struct Expr* expression;
    } grouping;

    struct {
      LoxObject* value;
    } literal;

    struct {
      struct Expr* callee;
      Token* paren;
      unsigned int num_args;
      struct Expr** args;
    } call;

    struct {
      Token* op;
      struct Expr* right;
    } unary;

    struct {
      Token* name;
    } var;

    struct {
      Token* name;
      struct Expr* value;
    } assign;
  } as;

  ExprType type;
  FileLoc* fileloc;
} Expr;

typedef enum StmtType {
  STMT_BLOCK,
  STMT_CLASS,
  STMT_EXPRESSION,
  STMT_IF,
  STMT_PRINT,
  STMT_RETURN,
  STMT_VAR,
  STMT_WHILE_LOOP,
  STMT_FUNCTION,
} StmtType;

typedef struct Stmt {
  union {
    struct {
      struct Stmt** statements;
      size_t length;
    } block;

    struct {
      
    } class_declaration;

    struct {
      Expr* expression;
    } expression;

    struct {
      Token* name;
      Token** params;
      unsigned int arity;
      Stmt* body;
    } function;

    struct {
      Expr* condition;
      struct Stmt* then_branch;
      struct Stmt* else_branch;
    } if_statement;

    struct {
      Expr* expression;
    } print; 

    struct {
      Token keyword;
      Expr* value;
    } return_stmt;

    struct {
      Token* name;
      Expr* initializer;
    } var;

    struct {
      Expr* condition;
      struct Stmt* body;
    } while_loop;
  } as;
  StmtType type;
} Stmt;

typedef struct Parser {
  const char* source;
  Token** tokens;
  size_t current;
  size_t num_stmts;
  Stmt** stmts;
  bool need_repl_resolution;
} Parser;


typedef struct Scanner {
  // points to the next character
  size_t current;
  // points to the start of a prospective token
  size_t start;
  size_t line;
  char *source;

  Array* tokens_array;
  // position of the last line character
  size_t last_line;
} Scanner;

typedef struct Interpreter {
  Parser* parser;
  Scanner* scanner;
  Environment* globals;
} Interpreter;

typedef struct FileLoc {
   size_t line;
   size_t start;
   size_t end;
} FileLoc;

#endif
