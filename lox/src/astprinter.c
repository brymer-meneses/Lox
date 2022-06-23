#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#include "lox/expr.h"
#include "lox/astprinter.h"
#include "lox/types.h"
#include "lox/literal.h"

void parenthesize(const char* name, const Expr* *exprs);

char* ast_to_string(const Expr *expr) {
  switch (expr->type) {
    case EXPR_BINARY:
    case EXPR_GROUPING:
    case EXPR_LITERAL:
      if (expr->value == NULL) {
        return "Null";
      } else {
        return literal_to_string(expr->value);
      }
    break;
    case EXPR_UNARY:
    break;
  }
}

void parenthesize(const char* name, const Expr* *exprs) {

  char result[2048] = "";

  strcat(result, "(");
  strcat(result, name);

  int length = sizeof(**exprs)/sizeof(Expr*);
  for (int i=0; i<length; i++) {
    strcat(result, " ");
    strcat(result, ast_to_string(exprs[i]));
  }


  strcat(result, ")");

}
