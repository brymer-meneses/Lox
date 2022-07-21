#ifndef EXPR_H
#define EXPR_H

#include "token.h"


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

Expr* binary__init(Expr* left, Token* op, Expr *right);
Expr* grouping__init(Expr* expr);
Expr* literal__init(LoxObject* value);
Expr* unary__init(Token* op, Expr* right);
Expr* var__init(Token* name);
Expr* assign__init(Token* name, Expr* value);
Expr* logical__init(Expr* left, Token* op, Expr* right);
Expr* call__init(Expr* callee, Token* paren, unsigned int num_args, Expr** args);

void expr__free(Expr* expr);
#endif
