
#include "lox/declarations.h"
#include "lox/environment.h"
#include "lox/lox.h"
#include "lox/object.h"
#include "lox/parser.h"
#include "lox/scanner.h"
#include "lox/token.h"
#include "lox/expr.h"
#include "lox/interpreter.h"
#include "lox/stmt.h"
#include "lox/error.h"

#include "tools/array.h"
#include "tools/fileloc.h"
#include "tools/hashmap.h"
#include "tools/utils.h"

#include "string.h"
#include "stdio.h"
#include "assert.h"
#include "math.h"

static LoxObject* evaluate_expression(Environment* env, Expr *expr);
static LoxObject* evaluate_statement(Environment* env, Stmt* stmt);


static void interpret(Environment* env, Stmt** statements, size_t num_stmts);

static Interpreter* interpreter;

Interpreter* interpreter__init() {
  interpreter = malloc(1 * sizeof(Interpreter));
  interpreter->globals = environment__init(NULL);
  return interpreter;
}

void interpreter__run(const char* source) {
  Scanner* scanner = scanner__init((char*) source);
  Token** tokens = scanner__scan();
  Parser* parser = parser__init(tokens);
  Stmt** statements = parser__parse();

  interpreter->parser = parser;
  interpreter->scanner = scanner;

  interpret(interpreter->globals, statements, parser->num_stmts);
}

void interpreter__free() {

}

void interpreter__execute(Environment* env, Stmt* stmt) {
  assert(stmt != NULL);

  evaluate_statement(env, stmt);
}

static void interpret(Environment* env, Stmt** statements, size_t num_stmts) {
  assert(env != NULL);

  for (size_t i=0; i<num_stmts && !lox->status.had_runtime_error ;i++) {
    interpreter__execute(env, statements[i]);
  }

}


static void execute_block(Stmt* stmt, Environment* env) {
  for (size_t i=0; i<stmt->as.block.length; i++) {
    interpreter__execute(env, stmt->as.block.statements[i]);
  }
}


static void check_same_operands(LoxType t1, LoxType t2, FileLoc* fl) {
    if (t1 != LOX_NUMBER || t2 != LOX_NUMBER) {
      report(fl, "Invalid binary operation between %s and %s", loxtype__to_string(t1), loxtype__to_string(t2));
    }
}

static LoxObject* evaluate_expression(Environment* env, Expr *expr) {
  assert(expr != NULL);

  switch (expr->type) {
    case EXPR_BINARY: {
      LoxObject* left  = evaluate_expression(env, expr->as.binary.left);
      LoxObject* right = evaluate_expression(env, expr->as.binary.right);
      const Token* op = expr->as.binary.op;

      FileLoc* fl = expr->fileloc;
      LoxObject* result = NULL;

      switch (op->type) {
        case MINUS:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject__number(left->as.number - right->as.number, fl);
          break;
        case SLASH:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject__number(left->as.number / right->as.number, fl);
          break;
        case STAR:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject__number(left->as.number * right->as.number, fl);
          break;
        case PLUS: {
            if (left->type == LOX_STRING  && right->type == LOX_STRING) {
              result = loxobject__string(str_concat(left->as.string, right->as.string), fl);
            }

            if (left->type == LOX_NUMBER  && right->type == LOX_NUMBER) {
              result = loxobject__number(left->as.number + right->as.number, fl);
            }
            if (left->type != right->type) 
              report(fileloc__range(3, left->fl, op->fileloc, right->fl), "The operation + is invalid for types: %s and %s", loxtype__to_string(left->type), loxtype__to_string(right->type));
          }
          break;
        case POW:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject__number(pow(left->as.number, right->as.number), fl);
          break;
        case GREATER:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject__boolean(left->as.number > right->as.number, fl);
          break;
        case GREATER_EQUAL:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject__boolean(left->as.number >= right->as.number, fl);
          break;
        case LESS:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject__boolean(left->as.number < right->as.number, fl);
          break;
        case LESS_EQUAL:
          check_same_operands(left->type, right->type, op->fileloc);
          result = loxobject__boolean(left->as.number <= right->as.number, fl);
          break;
        case BANG_EQUAL:
          result = loxobject__boolean(!loxobject__isequal(left, right), fl);
          break;
        case EQUAL_EQUAL:
          result = loxobject__boolean(loxobject__isequal(left, right), fl);
          break;
        default:
          break;
      }

      return result;
      } break;
    case EXPR_UNARY:  {
      LoxObject* right = evaluate_expression(env, expr->as.unary.right);
      LoxObject* result = NULL;

      switch (expr->as.unary.op->type) {
        case MINUS:
           result = loxobject__number(-right->as.number, right->fl);
           break;
        case BANG:
           result = loxobject__boolean(!right->as.boolean, right->fl);
           break;
        default:
           break;
      }

      return result;
      } break;
    case EXPR_GROUPING: 
      return evaluate_expression(env, expr->as.grouping.expression); 
      break;
    case EXPR_LITERAL: 
      return expr->as.literal.value; 
      break;
    case EXPR_VAR:
      return environment__get(env, expr->as.var.name);
      break;
    case EXPR_ASSIGN: {
      LoxObject* value = evaluate_expression(env, expr->as.assign.value);
      environment__assign(env, expr->as.assign.name, value);
      return value;
    } break;
    case EXPR_LOGICAL: {
      LoxObject* left = evaluate_expression(env, expr->as.logical.left);

      if (expr->as.logical.op->type == OR) {
        if (loxobject__istruthy(left)) return left;
      } else {
        if (!loxobject__istruthy(left)) return left;
      }

      return evaluate_expression(env, expr->as.logical.right);
    }
    case EXPR_CALL: {
      LoxObject* callee = evaluate_expression(env, expr->as.call.callee);

      Array* arguments_array = array__init(sizeof(Expr*));

      for (unsigned int i=0; i<expr->as.call.num_args; i++) {
        array__append(arguments_array, expr->as.call.args[i]);
      }

      if (callee->type != LOX_FUNCTION)
        report(callee->fl, "Lox can only call functions and classes.");
      
    }
  }

  return NULL;
}

static LoxObject* evaluate_statement(Environment* env, Stmt* stmt) {
  assert(stmt != NULL);

  switch (stmt->type) {
    case STMT_EXPRESSION: {
      LoxObject* obj =  evaluate_expression(env, stmt->as.expression.expression);
      return obj;
    } break;
    case STMT_PRINT: {
      LoxObject* value = evaluate_expression(env, stmt->as.print.expression);
      printf("%s\n", loxobject__to_string(value));
      return value;
    } break;
    case STMT_VAR: {
      LoxObject* value = NULL;
      if (stmt->as.var.initializer != NULL) {
        value = evaluate_expression(env, stmt->as.var.initializer);
      }
      environment__define(env, stmt->as.var.name->lexeme, value);
      return value;
    } break;
    case STMT_BLOCK: {
      execute_block(stmt, environment__init(env));
    }; break;
    case STMT_IF: { 
      if (loxobject__istruthy(evaluate_expression(env, stmt->as.if_statement.condition))) {
        interpreter__execute(env, stmt->as.if_statement.then_branch);
      } else if (stmt->as.if_statement.else_branch != NULL) {
        interpreter__execute(env, stmt->as.if_statement.else_branch);
      }
    } break;
    case STMT_WHILE_LOOP: {
      while (loxobject__istruthy(evaluate_expression(env, stmt->as.while_loop.condition))) {
        interpreter__execute(env, stmt->as.while_loop.body);
      };
    } break;
      break;
  }

  return NULL;
}
