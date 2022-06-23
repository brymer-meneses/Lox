#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "lox/utils.h"
#include "ctype.h"

char* substring(const char* source, const size_t begin, const size_t end) {
  const size_t length = (end - begin + 1) ; // take into account that ending char '\0'

  char* allocated_str = (char*) malloc(length * sizeof(char));

  size_t i = 0;
  while (i < length && source[i] != '\0') {
    allocated_str[i] = source[i + begin];
    i++;
  }
  allocated_str[length] = '\0';
  return allocated_str;
}

bool is_boolean(const char* string) {

  bool istrue =  strcmp(string, "true") == 0;
  bool isfalse = strcmp(string, "false") == 0;

  return istrue || isfalse;
}

bool is_real_number(const char *string) {
  const unsigned int string_length = strlen(string); 
  
  bool has_negative_sign = false;
  bool has_decimal_point = false;

  for (int i=0; i<string_length; i++) {
    char c = string[i];
    switch (c) {
      case '.':
        if (has_decimal_point) 
          return false;
        else 
          has_decimal_point = true;
          continue;
        break;
      case '-':
        if (has_negative_sign || i != 0) 
          return false;
        else
          has_negative_sign = true;
          continue;
        break;
      default:
        if (!isdigit(c))
          return false;
        break;
    }

  }
  return true;
}
