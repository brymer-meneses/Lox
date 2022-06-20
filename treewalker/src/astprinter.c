#include "astprinter.h"
#include "expr.h"
#include "stdio.h"
#include "stdarg.h"
#include <string.h>

char* ast_to_string(const Expr *expr) {
  switch (expr->type) {
    case Binary:
    case Grouping:
    case Literal:
    case Unary:
    break;
  }
}

void parenthesize(const char* name, const Expr* *exprs) {

  char* result = "";

  strcat(result, "(");

  int length = sizeof(**exprs)/sizeof(Expr*);

  for (int i=0; i<length; i++) {
    strcat(result, " ");
    strcat(result, ast_print(exprs[i]));

  }

  

  strcat(result, ")");

}
