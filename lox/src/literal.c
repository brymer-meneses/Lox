#include "lox/literal.h"
#include "lox/token.h"

#include "stdlib.h"
#include "stdio.h"

double literal_parse_double(Literal literal) {
  return strtod(literal_parse_string(literal), NULL);
}

char* literal_parse_string(Literal literal) {
  return (char*) literal;
}

bool literal_parse_bool(Literal literal) {
  return (bool) literal;
}

char* literal_to_string(LiteralType type,Literal literal) {
  char output[sizeof(literal)];

  switch (type) {
    case LITERAL_NUMBER:
      snprintf(output, sizeof(literal), "%lf" , literal_parse_double(literal));
      break;
    case LITERAL_STRING:
      snprintf(output, sizeof(literal), "%s" , literal_parse_string(literal));
      break;
    case LITERAL_BOOLEAN:
      snprintf(output, sizeof(literal), "%s" , literal_parse_string(literal));
      break;
  }
  return "";
}
