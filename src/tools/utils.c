#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
#include "stdarg.h"
#include "assert.h"

#include "tools/utils.h"
#include "lox/scanner.h"
#include "lox/lox.h"

static char* compat_strsep(char** stringp, const char* delim);

char* substring(const char* source, const size_t begin, const size_t end) {
  assert(source != NULL);
  assert(end < strlen(source));

  const size_t length = (end - begin + 1) ; // take into account that ending char '\0'

  char* allocated_str = malloc(length * sizeof(char));

  size_t i = 0;
  while (i < length && source[i] != '\0') {
    allocated_str[i] = source[i + begin];
    i++;
  }
  allocated_str[length] = '\0';
  return allocated_str;
}


bool is_boolean(const char* string) {
  if (strcmp(string, "true") == 0 || strcmp(string, "false") == 0) {
    return true;
  }

  return false;
}

bool is_real_number(const char *string) {
  const unsigned int string_length = strlen(string); 
  
  bool has_negative_sign = false;
  bool has_decimal_point = false;

  for (unsigned int i=0; i<string_length; i++) {
    char c = string[i];
    switch (c) {
      case '.':
        if (has_decimal_point) {
          return false;
        } else {
          has_decimal_point = true;
          continue;
        }
        break;
      case '-':
        if (has_negative_sign || i != 0) {
          return false;
        } else {
          has_negative_sign = true; 
            continue;
        }
        break;
      default:
        if (!isdigit(c))
          return false;
        break;
    }

  }
  return true;
}

char* read_file(const char* filename) {
  FILE* file = fopen(filename, "rt");

  if (!file) return NULL;

  fseek(file, 0, SEEK_END);
  size_t length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char* buffer = (char *) malloc(length + 1);
  buffer[length] = '\0';

  fread(buffer, 1, length, file);

  fclose(file);
  return buffer;
}

char** str_split(const char* str, const char* delim) {
  size_t i = 0;
  size_t str_arr_size = 8;
  char** str_arr = malloc(str_arr_size * sizeof(char*));
  char* str_copy = strdup(str);

  char* substr = compat_strsep(&str_copy, delim);
  while ( substr != NULL ) {
    str_arr[i] = substr;
    i++;

    if (i >= str_arr_size) {
      str_arr_size *= 2;
      str_arr = realloc(str_arr, str_arr_size * sizeof(char*));
    }
    substr = strsep(&str_copy, delim);
  }


  return str_arr;
}

char* str_concat(const char* str1, const char* str2) {
  const size_t length = (strlen(str1) + strlen(str2)) * sizeof(char) + 1;
  char* sum_str = malloc(length);

  snprintf(sum_str, length, "%s%s", str1, str2);

  return sum_str;
}

// cross-platform strsep, this is for compatibility.
// credits: https://stackoverflow.com/a/8514474
static char* compat_strsep(char** stringp, const char* delim) {
  char* start = *stringp;
  char* p;

  p = (start != NULL) ? strpbrk(start, delim) : NULL;

  if (p == NULL) {
    *stringp = NULL;
  } else {
    *p = '\0';
    *stringp = p + 1;
  }

  return start;
}
