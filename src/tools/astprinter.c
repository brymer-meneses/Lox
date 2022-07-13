#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#include "tools/astprinter.h"

#include "lox/expr.h"
#include "lox/object.h"
#include "lox/token.h"


char* parenthesize(const char* name, const Expr* left, const Expr* right);

char* expr_to_string(const Expr *expr) {
  if (expr == NULL) return "";

  switch (expr->type) {
    case EXPR_UNARY:
      return parenthesize(expr->data.unary.operation->lexeme, NULL, expr->data.unary.right);
      break;
    case EXPR_BINARY:
      return parenthesize(expr->data.binary.operation->lexeme, expr->data.binary.left, expr->data.binary.right);
      break;
    case EXPR_GROUPING:
      return parenthesize("group", expr->data.grouping.expression, NULL);
      break;
    case EXPR_LITERAL:
      return loxobject_to_string(expr->data.literal.value);
      break;
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
