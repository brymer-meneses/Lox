#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

#include "lox/object.h"
#include "tools/error.h"
#include "tools/fileloc.h"
#include "tools/utils.h"

char* loxobject_to_string(LoxObject* obj) {
  if (obj == NULL) return "NIL";

  assert(obj != NULL);

  char* output = malloc(sizeof(obj->data));

  switch (obj->type) {
    case LOX_NUMBER:
      sprintf(output, "%lf" , obj->data.number);
      break;
    case LOX_BOOLEAN: 
      obj->data.boolean == true 
        ? sprintf(output, "true") 
        : sprintf(output, "false");
      break;
    case LOX_STRING:
      sprintf(output, "%s" , obj->data.string);
      break;
    case LOX_NIL:
      sprintf(output, "NIL");
      break;
  }
  return output;
}
char* loxtype_to_string(LoxType type) {
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

LoxObject* loxobject_init(LoxType type, char* lexeme, FileLoc* fl) {
  assert(lexeme != NULL);
  assert(fl != NULL);

  LoxObject* obj = malloc( 1 * sizeof(LoxObject) );
  obj->type = type;
  obj->fl = fl;
  switch (type) {
    case LOX_NUMBER:
      assert(is_real_number(lexeme));
      obj->data.number = strtod(lexeme, NULL);
      break;
    case LOX_BOOLEAN: 
      assert(is_boolean(lexeme));
      strcmp(lexeme, "true") == 0 
        ? (obj->data.boolean = true)
        : (obj->data.boolean = false);
      break;
    case LOX_STRING:
      obj->data.string = strdup(lexeme);
      break;
    case LOX_NIL:
      free(obj);
      return NULL;
      break;
  }
  return obj;
} 

LoxObject* loxobject_boolean(bool value, FileLoc* fl) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_BOOLEAN;
  obj->data.boolean = value;
  obj->fl = fl;
  return obj;
};

LoxObject* loxobject_string(char* value, FileLoc* fl) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_STRING;
  obj->data.string = value;
  obj->fl = fl;
  return obj;
}

LoxObject* loxobject_number(double value, FileLoc* fl) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_NUMBER;
  obj->data.number = value;
  obj->fl = fl;
  return obj;
}

