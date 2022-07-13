#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "declarations.h"


typedef enum ExprType {
  EXPR_BINARY,
  EXPR_UNARY,
  EXPR_LITERAL,
  EXPR_GROUPING,
} ExprType;


typedef struct Expr {
  union {
    struct {
      struct Expr* left;
      struct Expr* right;
      Token* operation;
    } binary;

    struct {
      struct Expr* expression;
    } grouping;

    struct {
      LoxObject* value;
    } literal;

    struct {
      Token* operation;
      struct Expr* right;
    } unary;
  } data;

  ExprType type;
  FileLoc* fileloc;
} Expr;

Expr* binary_init(Expr* left, Token* op, Expr *right);
Expr* grouping_init(Expr* expr);
Expr* literal_init(LoxObject* value);
Expr* unary_init(Token* op, Expr* right);

#endif
