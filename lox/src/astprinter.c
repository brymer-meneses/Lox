#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#include "lox/expr.h"
#include "lox/astprinter.h"
#include "lox/types.h"
#include "lox/literal.h"

char* parenthesize(const char* name, const Expr* left, const Expr* right);

char* expr_to_string(const Expr *expr) {
  switch (expr->type) {
    case EXPR_UNARY:
      return parenthesize(expr->op.lexeme, NULL, expr->right);
    case EXPR_BINARY:
      return parenthesize(expr->op.lexeme, expr->left, expr->right);
    case EXPR_GROUPING:
      return parenthesize("group", expr->left, NULL);
    case EXPR_LITERAL:
      return literal_to_string(expr->value);
   }
};

char* parenthesize(const char* name, const Expr* left, const Expr* right) {

  char* result = malloc(2048 * sizeof(char));

  strcat(result, "(");
  strcat(result, name);

  if (left != NULL) {
    strcat(result, " ");
    strcat(result, expr_to_string(left));
  }

  if (right != NULL) {
    strcat(result, " ");
    strcat(result, expr_to_string(right));
  }

  strcat(result, ")");
  return result;
}
