
#include "lox/declarations.h"
#include "lox/environment.h"
#include "lox/lox.h"
#include "lox/object.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/interpreter.h"
#include "lox/stmt.h"

#include "tools/array.h"
#include "tools/fileloc.h"
#include "tools/hashmap.h"
#include "tools/utils.h"
#include "tools/error.h"

#include "string.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"

static LoxObject* interpret_expr(Environment* env, Expr *expr);
static LoxObject* interpret_stmt(Environment* env, Stmt* stmt);
static bool is_equal(LoxObject* obj1, LoxObject* obj2);


LoxObject* execute(Environment* env, Stmt* stmt) {
  assert(stmt != NULL);

  return interpret_stmt(env, stmt);
}

void execute_block(Array* statements, Environment* env) {
  for (size_t i=0; i<statements->curr_size; i++) {
    execute(env, statements->elements[i]);
  }
}

void interpret(Stmt** statements, Environment* env, size_t num_stmts) {
  assert(env != NULL);

  for (size_t i=0; i<num_stmts && !lox__get()->status.had_runtime_error ;i++) {
    execute(env, statements[i]);
  }

}

static bool is_equal(LoxObject* obj1, LoxObject* obj2) {

  switch (obj1->type) {
    case LOX_NUMBER:
      return obj1->as.number == obj2->as.number;
    case LOX_STRING:
      return strcmp(obj1->as.string, obj2->as.string) == 0;
    case LOX_BOOLEAN:
      return obj1->as.boolean == obj2->as.boolean;
    default:
      return false;
  }
}


static void check_same_operands(LoxType t1, LoxType t2, FileLoc* fl) {
    if (t1 != LOX_NUMBER || t2 != LOX_NUMBER) {
      report(fl, "Invalid binary operation between %s and %s", loxtype_to_string(t1), loxtype_to_string(t2));
    }
}

static LoxObject* interpret_expr(Environment* env, Expr *expr) {
  assert(expr != NULL);

  switch (expr->type) {
    case EXPR_BINARY: {
      LoxObject* left  = interpret_expr(env, expr->as.binary.left);
      LoxObject* right = interpret_expr(env, expr->as.binary.right);
      const Token* op = expr->as.binary.op;

      FileLoc* fl = expr->fileloc;
      LoxObject* result = NULL;

      switch (op->type) {
        case MINUS:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_number(left->as.number - right->as.number, fl);
          break;
        case SLASH:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_number(left->as.number / right->as.number, fl);
          break;
        case STAR:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_number(left->as.number * right->as.number, fl);
          break;
        case PLUS: {
            if (left->type == LOX_STRING  && right->type == LOX_STRING) {
              result = loxobject_string(str_concat(left->as.string, right->as.string), fl);
            }

            if (left->type == LOX_NUMBER  && right->type == LOX_NUMBER) {
              result = loxobject_number(left->as.number + right->as.number, fl);
            }
            if (left->type != right->type) 
              report(fileloc_range(3, left->fl, op->fileloc, right->fl), "The operation + is invalid for types: %s and %s", loxtype_to_string(left->type), loxtype_to_string(right->type));
          }
          break;
        case POW:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_number(pow(left->as.number, right->as.number), fl);
          break;
        case GREATER:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_boolean(left->as.boolean > right->as.boolean, fl);
          break;
        case GREATER_EQUAL:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_boolean(left->as.boolean >= right->as.boolean, fl);
          break;
        case LESS:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_boolean(left->as.boolean < right->as.boolean, fl);
          break;
        case LESS_EQUAL:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject_boolean(left->as.boolean <= right->as.boolean, fl);
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

      return result;
      } break;
    case EXPR_UNARY:  {
      LoxObject* right = interpret_expr(env, expr->as.unary.right);
      LoxObject* result = NULL;

      switch (expr->as.unary.op->type) {
        case MINUS:
           result = loxobject_number(-right->as.number, right->fl);
           break;
        case BANG:
           result = loxobject_boolean(!right->as.boolean, right->fl);
           break;
        default:
           break;
      }

      return result;
      } break;
    case EXPR_GROUPING: 
      return interpret_expr(env, expr->as.grouping.expression); 
      break;
    case EXPR_LITERAL: 
      return expr->as.literal.value; 
      break;
    case EXPR_VAR:
      return environment_get(env, expr->as.var.name);
      break;
    case EXPR_ASSIGN: {
      LoxObject* value = interpret_expr(env, expr->as.assign.value);
      environment_assign(env, expr->as.assign.name, value);
      return value;
    } break;
  }

  return NULL;
}

static LoxObject* interpret_stmt(Environment* env, Stmt* stmt) {
  assert(stmt != NULL);

  switch (stmt->type) {
    case STMT_EXPRESSION: {
      LoxObject* obj =  interpret_expr(env, stmt->as.expression.expression);
      return obj;
    } break;
    case STMT_PRINT: {
      LoxObject* value = interpret_expr(env, stmt->as.print.expression);
      printf("%s\n", loxobject_to_string(value));
      return value;
    } break;
    case STMT_VAR: {
      LoxObject* value = NULL;
      if (stmt->as.var.initializer != NULL) {
        value = interpret_expr(env, stmt->as.var.initializer);
      }
      environment_define(env, stmt->as.var.name->lexeme, value);
      return value;
    } break;
    case STMT_BLOCK: {
      execute_block(stmt->as.block.statements, environment_init(env));
    }; break;
    case STMT_CONTROL_FLOW: { 

    } break;
    default:
      break;
  }

  return NULL;
}
