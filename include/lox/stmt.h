#ifndef STMT_H
#define STMT_H

#include "declarations.h"
#include "lox/expr.h"
#include "tools/array.h"

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
      Array* statements;
    } block;

    struct {
      
    } class_declaration;

    struct {
      Expr* expression;
    } expression;

    struct {
      Token name;
      Token* params;
      struct Stmt* body;
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

Stmt* stmt_expr_init(Expr* expression);
Stmt* stmt_while_init(Expr* condition, Stmt* body);
Stmt* stmt_return_init(Expr* keyword, Expr* value);
Stmt* stmt_print_init(Expr* expression);
Stmt* stmt_block_init(Array* statements);
Stmt* stmt_function_init(Token* name, Token* params, Stmt* body);
Stmt* stmt_vardecl_init(Token* name, Expr* initializer);
Stmt* stmt_control_flow_init(Expr* condition, Array* then_branch_stmts, Array* else_branch_stmts);
Stmt* stmt_if_init(Expr* condition, Stmt* then_branch, Stmt* else_branch);

void stmts_free(size_t num_stmts, Stmt** stmts);


#endif // !STMT_H
