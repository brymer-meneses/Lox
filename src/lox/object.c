#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"

#include "lox/object.h"
#include "tools/error.h"
#include "tools/fileloc.h"
#include "tools/utils.h"

char* loxobject_to_string( LoxObject* obj ) {
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

LoxObject* loxobject_init(LoxType type, char* lexeme, FileLoc* fl) {
  LoxObject* obj = malloc( 1 * sizeof(LoxObject) );
  obj->type = type;
  obj->fl = fl;
  switch (type) {
    case LOX_NUMBER:
      obj->data.number = strtod(lexeme, NULL);
      break;
    case LOX_BOOLEAN: 
      strcmp(lexeme, "true") == 0 
        ? (obj->data.boolean = true)
        : (obj->data.boolean = false);
      break;
    case LOX_STRING:
      obj->data.string = strdup(lexeme);
      break;
    case LOX_NIL:
      break;
  }
  return obj;
} 

LoxObject* loxobject_boolean(bool value) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_BOOLEAN;
  obj->data.boolean = value;
  return obj;
};

LoxObject* loxobject_string(char* value) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_STRING;
  obj->data.string = value;
  return obj;
}

LoxObject* loxobject_number(double value) {
  LoxObject* obj = malloc(1 * sizeof(LoxObject));
  obj->type = LOX_NUMBER;
  obj->data.number = value;
  return obj;
}

