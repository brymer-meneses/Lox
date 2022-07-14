#include "lox/environment.h"
#include "tools/fileloc.h"

#include "lox/declarations.h"
#include "lox/lox.h"
#include "lox/object.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/interpreter.h"
#include "tools/hashmap.h"
#include "tools/utils.h"
#include "lox/stmt.h"
#include "tools/error.h"

#include "string.h"
#include "stdio.h"
#include "assert.h"

static LoxObject* expr_evaluate(Expr *expr);
static void stmt_evaluate(Stmt* stmt);
static bool is_equal(LoxObject* obj1, LoxObject* obj2);

void execute(Stmt* stmt) {
  stmt_evaluate(stmt);
}

void interpret(Stmt** statements) {

  const size_t num_stmts = lox.parser.num_stmts;
  for (size_t i=0; i<num_stmts && !lox.had_runtime_error ;i++) {
    execute(statements[i]);
  }
}

static bool is_equal(LoxObject* obj1, LoxObject* obj2) {

  switch (obj1->type) {
    case LOX_NUMBER:
      return obj1->data.number == obj2->data.number;
    case LOX_STRING:
      return strcmp(obj1->data.string, obj2->data.string) == 0;
    case LOX_BOOLEAN:
      return obj1->data.boolean == obj2->data.boolean;
    default:
      return false;
  }
}


static void check_same_operands(LoxType t1, LoxType t2, FileLoc* fl) {
    if (t1 != LOX_NUMBER || t2 != LOX_NUMBER) {
      report(fl, "Invalid binary operation between %s and %s", loxtype_to_string(t1), loxtype_to_string(t2));
    }
}

static LoxObject* expr_evaluate(Expr *expr) {
  assert(expr != NULL);

  switch (expr->type) {
    case EXPR_BINARY: {
      LoxObject* left = expr_evaluate(expr->data.Binary.left);
      LoxObject* right = expr_evaluate(expr->data.Binary.right);
      const Token* op = expr->data.Binary.operation;

      FileLoc* fl = expr->fileloc;
      LoxObject* result = NULL;

      switch (op->type) {
        case MINUS:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_number(left->data.number - right->data.number, fl);
          break;
        case SLASH:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_number(left->data.number / right->data.number, fl);
          break;
        case STAR:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_number(left->data.number * right->data.number, fl);
          break;
        case PLUS: {
            if (left->type == LOX_STRING  && right->type == LOX_STRING) {
              result = loxobject_string(str_concat(left->data.string, right->data.string), fl);
            }

            if (left->type == LOX_NUMBER  && right->type == LOX_NUMBER) {
              result = loxobject_number(left->data.number + right->data.number, fl);
            }
          }
          break;
        case GREATER:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_boolean(left->data.boolean > right->data.boolean, fl);
          break;
        case GREATER_EQUAL:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_boolean(left->data.boolean >= right->data.boolean, fl);
          break;
        case LESS:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_boolean(left->data.boolean < right->data.boolean, fl);
          break;
        case LESS_EQUAL:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_boolean(left->data.boolean <= right->data.boolean, fl);
          break;
        case BANG_EQUAL:
          result = loxobject_boolean(!is_equal(left, right), fl);
          break;
        case EQUAL_EQUAL:
          result = loxobject_boolean(is_equal(left, right), fl);
          break;
        default:
          break;
      }

      free(left);
      free(right);
      free(expr);
      return result;
      } break;
    case EXPR_UNARY:  {
      LoxObject* right = expr_evaluate(expr->data.Unary.right);
      LoxObject* result = NULL;

      switch (expr->data.Unary.operation->type) {
        case MINUS:
           result = loxobject_number(-right->data.number, right->fl);
           break;
        case BANG:
           result = loxobject_boolean(!right->data.boolean, right->fl);
           break;
        default:
           break;
      }

      free(right);
      free(expr);
      return result;
      } break;
    case EXPR_GROUPING: 
      return expr_evaluate(expr->data.Grouping.expression); 
      break;
    case EXPR_LITERAL: 
      return expr->data.Literal.value; 
      break;
    case EXPR_VAR_DECLARATION:
      return environment_get(expr->data.VarDecl.name);
      break;
    case EXPR_ASSIGN: {
      LoxObject* value = expr_evaluate(expr->data.Assign.value);
      environment_assign(expr->data.Assign.name, value);
      return value;
    } break;
  }

  return NULL;
}

static void stmt_evaluate(Stmt* stmt) {
  assert(stmt != NULL);

  switch (stmt->type) {
    case STMT_EXPRESSION: {
      LoxObject* obj = expr_evaluate(stmt->data.Expression.expression);
      if (lox.is_on_repl && !lox.had_error)
        printf("%s\n", loxobject_to_string(obj));
    } break;
    case STMT_PRINT: {
      LoxObject* value = expr_evaluate(stmt->data.Print.expression);
      printf("%s\n", loxobject_to_string(value));
    } break;
    case STMT_VAR: {
      LoxObject* value = NULL;
      if (stmt->data.Var.initializer != NULL) {
        value = expr_evaluate(stmt->data.Var.initializer);
      }
      environment_define(stmt->data.Var.name->lexeme, value);
      environment_dump();
    } break;
    default:
      break;
  }

}
