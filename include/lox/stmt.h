#ifndef STMT_H
#define STMT_H

#include "declarations.h"

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

typedef struct {
  union {
    struct {

    } block_stmt;

    struct {

    } class_stmt;

    struct {

    } expression_stmt;

    struct {

    } function_stmmt;

    struct {

    } if_stmt;

    struct {

    } print_stmt; 

    struct {

    } return_stmt;

    struct {

    } var_stmt;

    struct {

    } while_stmt;
  } data;
  StmtType type;
} Stmt;

#endif // !STMT_H
