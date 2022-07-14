#ifndef EXPR_H
#define EXPR_H

#include "token.h"
#include "declarations.h"


typedef enum ExprType {
  EXPR_BINARY,
  EXPR_UNARY,
  EXPR_LITERAL,
  EXPR_GROUPING,
  EXPR_VAR_DECLARATION,
  EXPR_ASSIGN,
} ExprType;


typedef struct Expr {
  union {
    struct {
      struct Expr* left;
      struct Expr* right;
      Token* operation;
    } Binary;

    struct {
      struct Expr* expression;
    } Grouping;

    struct {
      LoxObject* value;
    } Literal;

    struct {
      Token* operation;
      struct Expr* right;
    } Unary;

    struct {
      Token* name;
    } VarDecl;

    struct {
      Token* name;
      struct Expr* value;
    } Assign;
  } data;

  ExprType type;
  FileLoc* fileloc;
} Expr;

Expr* binary_init(Expr* left, Token* op, Expr *right);
Expr* grouping_init(Expr* expr);
Expr* literal_init(LoxObject* value);
Expr* unary_init(Token* op, Expr* right);
Expr* vardecl_init(Token* name);
Expr* assign_init(Token* name, Expr* value);

#endif
