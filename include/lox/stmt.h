#ifndef STMT_H
#define STMT_H

#include "declarations.h"
#include "lox/expr.h"

typedef enum StmtType {
  STMT_BLOCK,
  STMT_CLASS,
  STMT_EXPRESSION,
  STMT_IF,
  STMT_PRINT,
  STMT_RETURN,
  STMT_VAR,
  STMT_WHILE,
} StmtType;

typedef struct Stmt {
  union {
    struct {
      struct Stmt* statements;
    } Block;

    struct {
      
    } Class;

    struct {
      Expr* expression;
    } Expression;

    struct {
      Token name;
      Token* params;
      struct Stmt* body;
    } Function;

    struct {
      Expr* condition;
      struct Stmt* then_branch;
      struct Stmt* else_branch;
    } If;

    struct {
      Expr* expression;
    } Print; 

    struct {
      Token keyword;
      Expr* value;
    } Return;

    struct {
      Token* name;
      Expr* initializer;
    } Var;

    struct {
      Expr* condition;
      struct Stmt* body;
    } While;
  } data;
  StmtType type;
} Stmt;

Stmt* stmt_expr_init(Expr* expression);
Stmt* stmt_while_init(Expr* condition, Stmt* body);
Stmt* stmt_return_init(Expr* keyword, Expr* value);
Stmt* stmt_print_init(Expr* expression);
Stmt* stmt_block_init(Expr* statements);
Stmt* stmt_function_init(Token name, Token* params, Stmt* body);

#endif // !STMT_H
