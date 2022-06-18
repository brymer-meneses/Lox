#include "stdlib.h"
#include "stdio.h"
#include "string.h"

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

