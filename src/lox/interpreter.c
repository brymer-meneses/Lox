
#include "lox/declarations.h"
#include "lox/filelocation.h"
#include "lox/lox.h"
#include "lox/object.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/interpreter.h"
#include "tools/utils.h"
#include "tools/error.h"

#include "string.h"
#include "stdio.h"
#include "assert.h"

static LoxObject expr_evaluate(Expr *expr);
static bool is_equal(LoxObject obj1, LoxObject obj2);

void interpret(Expr* expression) {
  if (expression == NULL) {
    printf("NIL\n");
    return;
  }

  LoxObject value = expr_evaluate(expression);
  printf("%s\n", loxobject_to_string(value));
}

static bool is_equal(LoxObject obj1, LoxObject obj2) {

  switch (obj1.type) {
    case NUMBER:
      return obj1.data.number == obj2.data.number;
    case STRING:
      return strcmp(obj1.data.string, obj2.data.string) == 0;
    case TRUE:
    case FALSE:
      return obj1.data.boolean == obj2.data.boolean;
    default:
      return false;
  }
}


static void check_same_types(TokenType expected, Token op, TokenType t1, TokenType t2) {
    if (t1 != expected || t2 != expected) {
      raise_invalid_binary_operation_error(op, t1, t2);
    }
}

static LoxObject expr_evaluate(Expr *expr) {
  assert(expr != NULL);
  switch (expr->type) {
    case EXPR_BINARY: {
      LoxObject left = expr_evaluate(expr->data.binary.left);
      LoxObject right = expr_evaluate(expr->data.binary.right);
      const Token op = expr->data.binary.operation;

      switch (op.type) {
        case MINUS:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_double(left.data.number - right.data.number);
          break;
        case SLASH:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_double(left.data.number / right.data.number);
          break;
        case STAR:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_double(left.data.number * right.data.number);
          break;
        case PLUS: {
          if (left.type == NUMBER && right.type == NUMBER) {
            return encode_double(left.data.number + right.data.number);
          }

          if (left.type == STRING && right.type == STRING) {
            return encode_string(str_concat(left.data.string, right.data.string));
          }
          raise_invalid_binary_operation_error(op, left.type, right.type);
        } break;
        case GREATER:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_bool(left.data.number > right.data.number);
          break;
        case GREATER_EQUAL:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_bool(left.data.number >= right.data.number);
          break;
        case LESS:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_bool(left.data.number < right.data.number);
          break;
        case LESS_EQUAL:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_bool(left.data.number <= right.data.number);
          break;
        case BANG_EQUAL:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_bool(!is_equal(left, right));
          break;
        case EQUAL_EQUAL:
          check_same_types(NUMBER, op, left.type, right.type);
          return encode_bool(is_equal(left, right));
          break;
        default:
          return LOX_OBJECT_NULL;
          break;
      }
    }; break;
    case EXPR_UNARY: {
      LoxObject right = expr_evaluate(expr->data.unary.right);

      switch (expr->data.unary.operation.type) {
        case MINUS:
           return encode_double(-right.data.number);
           break;
        case BANG:
           return encode_bool(!right.data.boolean);
           break;
        default:
           return LOX_OBJECT_NULL;
           break;
      }
    }; break;
    case EXPR_GROUPING:
      return expr_evaluate(expr->data.grouping.expression);
      break;
    case EXPR_LITERAL:
      return expr->data.literal.value;
      break;
  }

  return LOX_OBJECT_NULL;
}
