#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"

#include "lox/literal.h"
#include "lox/token.h"
#include "lox/utils.h"

double literal_parse_double(Literal literal) {
  return strtod(literal, NULL);
}

bool literal_parse_bool(Literal literal) {
  bool istrue = strcmp(literal, "true") == 0;
  bool isfalse = strcmp(literal, "false") == 0;

  assert(istrue || isfalse);

  if (istrue) {
    return true; 
  }; 

  return false; 
}

LoxType literal_get_type(Literal literal) {
  bool isnum = is_real_number(literal);
  bool isbool = is_boolean(literal);

  if (isnum) {
    return LOX_NUMBER;
  } else if (isbool) {
    return LOX_BOOLEAN;
  } 
  return LOX_STRING;
}

char* literal_to_string(Literal literal) {
  char* output = malloc(sizeof(literal));

  switch (literal_get_type(literal)) {
    case LOX_NUMBER:
      snprintf(output, sizeof(literal), "%lf" , literal_parse_double(literal));
      break;
    case LOX_BOOLEAN:
      snprintf(output, sizeof(literal), "%d" , literal_parse_bool(literal));
      break;
    case LOX_STRING:
      return (char*) literal;
      break;
  }
  return output;
}
