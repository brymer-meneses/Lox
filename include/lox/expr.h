#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "declarations.h"


typedef enum ExprType {
  EXPR_BINARY,
  EXPR_UNARY,
  EXPR_LITERAL,
  EXPR_GROUPING,
  EXPR_VAR,
  EXPR_ASSIGN,
} ExprType;


typedef struct Expr {
  union {
    struct {
      struct Expr* left;
      struct Expr* right;
      Token* op;
    } binary;

    struct {
      struct Expr* expression;
    } grouping;

    struct {
      LoxObject* value;
    } literal;

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

Expr* binary_init(Expr* left, Token* op, Expr *right);
Expr* grouping_init(Expr* expr);
Expr* literal_init(LoxObject* value);
Expr* unary_init(Token* op, Expr* right);
Expr* var_init(Token* name);
Expr* assign_init(Token* name, Expr* value);

void expr_free(Expr* expr);
#endif
