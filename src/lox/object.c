#include "lox/core.h"
#include "lox/environment.h"
#include "lox/interpreter.h"

#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

#include "lox/object.h"
#include "lox/error.h"
#include "lox/fileloc.h"
#include "tools/utils.h"

char* loxobject__to_string(LoxObject* obj) {
  if (obj == NULL) return "NIL";

  assert(obj != NULL);

  char* output = malloc(sizeof(obj->as));

  switch (obj->type) {
    case LOX_NUMBER:
      sprintf(output, "%lf" , obj->as.number);
      break;
    case LOX_BOOLEAN: 
      obj->as.boolean == true 
        ? sprintf(output, "true") 
        : sprintf(output, "false");
      break;
    case LOX_STRING:
      sprintf(output, "%s" , obj->as.string);
      break;
    case LOX_NIL:
      sprintf(output, "NIL");
      break;
  }
  return output;
}
char* loxtype__to_string(LoxType type) {
  switch (type) {
    case LOX_STRING:
      return "string";
      return "boolean";
    case LOX_NUMBER:
      return "number";
      return "boolean";
    case LOX_BOOLEAN:
      return "boolean";
      break;
    case LOX_NIL:
      return "nil";
      break;
  }
}

bool loxobject__istruthy(LoxObject* obj) {
  if (obj == NULL) return false;

  switch (obj->type) {
    case LOX_NUMBER:
    case LOX_STRING:
      return false;
      break;
    case LOX_BOOLEAN:
      return obj->as.boolean;
      break;
    case LOX_NIL:
      return false;
      break;
  }
}

bool loxobject__isequal(LoxObject* obj1, LoxObject* obj2) {

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

LoxObject* loxobject__init(LoxType type, char* lexeme, FileLoc* fl) {
  assert(lexeme != NULL);
  assert(fl != NULL);

  LoxObject* obj = malloc( 1 * sizeof(LoxObject) );
  obj->type = type;
  obj->fl = fl;
  switch (type) {
    case LOX_NUMBER:
      assert(is_real_number(lexeme));
      obj->as.number = strtod(lexeme, NULL);
      break;
    case LOX_BOOLEAN: 
      assert(is_boolean(lexeme));
      strcmp(lexeme, "true") == 0 
        ? (obj->as.boolean = true)
        : (obj->as.boolean = false);
      break;
    case LOX_STRING:
      obj->as.string = strdup(lexeme);
      break;
    case LOX_NIL:
      return NULL;
      break;
  }
  return obj;
} 

LoxObject* loxobject__boolean_init(bool value, FileLoc* fl) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_BOOLEAN;
  obj->as.boolean = value;
  obj->fl = fl;
  return obj;
};

LoxObject* loxobject__string_init(char* value, FileLoc* fl) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_STRING;
  obj->as.string = value;
  obj->fl = fl;
  return obj;
}

LoxObject* loxobject__number_init(double value, FileLoc* fl) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_NUMBER;
  obj->as.number = value;
  obj->fl = fl;
  return obj;
}

LoxObject* loxobject__nil_init(FileLoc* fl) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_NIL;
  obj->fl = fl;
  return obj;
}

LoxObject* loxobject__callable_init(char* to_string, unsigned int arity, LoxObject** args, FileLoc* fl) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_CALLABLE;
  obj->as.callable.arity = arity;
  obj->as.callable.arguments = args;
  obj->as.callable.to_string = to_string;
  obj->fl = fl;
  return obj;
}

LoxObject* loxobject__function_init(Stmt* declaration) {
  assert(declaration->type == STMT_FUNCTION);
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_FUNCTION;
  obj->as.function.declaration = declaration;
  return obj;
}

LoxObject* loxobject__call(LoxObject** obj, unsigned int num_args) {

  // Environment* env = environment__init(interpreter__get_globals());
  // const unsigned int arity = obj->as.function.declaration->as.function.arity;
  //
  // for (unsigned int i=0; i< arity; i++) {
  //   environment__define(env, obj->as.function.declaration->as.function.params[i]->lexeme,
  //                       NULL
  //                       );
  // }
}



void loxobject__free(LoxObject* obj) {
  if (obj == NULL) return;
  switch (obj->type) {
    case LOX_STRING:
      free(obj->as.string);
      break;
    default:
      break;
  }
  free(obj->fl);
  free(obj);
}
