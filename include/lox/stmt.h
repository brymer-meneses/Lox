#ifndef STMT_H
#define STMT_H

#include "lox/expr.h"

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
      size_t num_params;
      struct Stmt** body;
      size_t num_body_stmts;
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
Stmt* stmt_block_init(size_t length, Stmt** statements);
Stmt* stmt_function_init(Token* name, Token** params, size_t num_params, Stmt** body, size_t num_body_stmts);

Stmt* stmt_vardecl_init(Token* name, Expr* initializer);
Stmt* stmt_if_init(Expr* condition, Stmt* then_branch, Stmt* else_branch);
Stmt* stmt_while_loop_init(Expr* condition, Stmt* body);

void stmts_free(size_t num_stmts, Stmt** stmts);


#endif // !STMT_H
