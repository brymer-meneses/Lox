#include "stdbool.h"
#include "stdio.h"

#include "lox/object.h"

char* loxobject_to_string(LoxObject obj) {
  char* output = malloc(sizeof(obj.data));

  switch (obj.type) {
    case NUMBER:
      sprintf(output, "%lf" , obj.data.number);
      break;
    case TRUE:
      sprintf(output, "true");
    case FALSE:
      sprintf(output, "false");
      break;
    case STRING:
      sprintf(output, "%s" , obj.data.string);
      break;
    case NIL:
      return "NULL";
      break;
    default:
      return (char*) ""; // leave for now
      break;
  }
  return output;

}


LoxObject encode_bool(bool value) {
  LoxObject obj;
  obj.data.boolean = value;

  if (value == true) {
    obj.type = TRUE;
  } else {
    obj.type = FALSE;
  }

  return obj;
}

LoxObject encode_string(const char* value) {
  LoxObject obj;
  obj.data.string = (char*) value;
  return obj;
}

LoxObject encode_double(double value) {
  LoxObject obj;
  obj.data.number = value;
  return obj;
}

