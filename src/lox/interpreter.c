
#include "lox/declarations.h"
#include "lox/filelocation.h"
#include "lox/lox.h"
#include "lox/object.h"
#include "lox/token.h"
#include "lox/interpreter.h"
#include "tools/utils.h"
#include "tools/error.h"

#include "string.h"
#include "stdio.h"

static LoxObject expr_evaluate(Expr *expr);
static bool is_equal(LoxObject obj1, LoxObject obj2);

void interpret(Expr* expression) {
  if (expression == NULL) return;

  LoxObject value = expr_evaluate(expression);
  if (value.type == NIL) {
    lox.had_runtime_error = true;
    return;
  }
  printf("%s\n", loxobject_to_string(value));
}

static bool is_equal(LoxObject obj1, LoxObject obj2) {
  TokenType obj1_type = obj1.type;
  TokenType obj2_type = obj2.type;

  if (obj1_type != obj2_type) return false;

  switch (obj1_type) {
    case NUMBER:
      return obj1.data.number == obj1.data.number;
    case STRING:
      return strcmp(obj1.data.string, obj2.data.string) == 0;
    default:
      return false;
  }

}

static LoxObject expr_evaluate(Expr *expr) {
  switch (expr->type) {
    case EXPR_BINARY: {
      LoxObject left = expr_evaluate(expr->left);
      LoxObject right = expr_evaluate(expr->right);

      if (left.type != right.type) {
        raise_invalid_binary_operation_error(expr->op, left.type, right.type);
      }

      switch (expr->op.type) {
        case MINUS:
          return encode_double(left.data.number - right.data.number);
        case SLASH:
          return encode_double(left.data.number / right.data.number);
        case STAR:
          return encode_double(left.data.number * right.data.number);
        case PLUS: {
          if (left.type == NUMBER && right.type == NUMBER) {
            return encode_double(left.data.number + right.data.number);
          }

          if (left.type == STRING && right.type == STRING) {
            return encode_string(str_concat(left.data.string, right.data.string));
          }
          break;
        }
        case GREATER:
          return encode_bool(left.data.number > right.data.number);
        case GREATER_EQUAL:
          return encode_bool(left.data.number >= right.data.number);
        case LESS:
          return encode_bool(left.data.number < right.data.number);
        case LESS_EQUAL:
          return encode_bool(left.data.number <= right.data.number);
        case BANG_EQUAL:
          return encode_bool(!is_equal(left, right));
        case EQUAL_EQUAL:
          return encode_bool(is_equal(left, right));
        default:
          return LOX_OBJECT_NULL;
      }


    }
    case EXPR_UNARY: {
      LoxObject right = expr_evaluate(expr->right);

      switch (expr->op.type) {
        case MINUS:
           return encode_double(-right.data.number);
        case BANG:
           return encode_bool(!right.data.boolean);
        default:
           return LOX_OBJECT_NULL;
      }

    };
    case EXPR_GROUPING:
      return expr_evaluate(expr);
    case EXPR_LITERAL:
      return expr->value;
      break;
  }

  return LOX_OBJECT_NULL;
}
