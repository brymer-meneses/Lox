#include "stdlib.h"
#include "stdio.h"

char *substring(char* source, char* buffer, size_t begin, size_t end) {
  size_t length = end - begin + 1;

  if (end < begin) return NULL;
  if (begin >= length) return NULL;

  char *substring = (char*) calloc(length, sizeof(char));
  if (!substring) {
    fprintf(stderr, "Memory allocation failed");
  }


  int i = 0;
  while (i < length) {
    substring[i] = source[begin+i];
    i++;
  }
  substring[length] = '\0';

  return substring;
}
