#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"
#include "stdarg.h"

#include "tools/utils.h"
#include "tools/fileloc.h"
#include "lox/scanner.h"
#include "lox/lox.h"

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

// Credits to: https://www.youtube.com/watch?v=CzAgM5bez-g
char* read_file(const char* filename) {
  FILE* file;
  file = fopen(filename, "r");
  if (file == NULL) return NULL;

  fseek(file, 0, SEEK_END);
  const int length = ftell(file);

  fseek(file, 0, SEEK_SET);

  char* string = malloc(sizeof(char) * (length + 1)); // accomodate for the null terminator

  char c;
  int i =0;
  while ( (c = fgetc(file))  != EOF) {
    string[i] = c;
    i++;
  }

  string[length] = '\0';
  return string;
}

char** str_split(const char* str, const char* delim) {
  size_t i = 0;
  size_t str_arr_size = 8;
  char** str_arr = malloc(str_arr_size * sizeof(char*));
  char* str_copy = strdup(str);

  char* substr = strtok(str_copy, delim);
  while ( substr != NULL ) {
    str_arr[i] = malloc(sizeof(char) * strlen(substr));
    str_arr[i] = substr;
    i++;

    if (i >= str_arr_size) {
      str_arr_size *= 2;
      str_arr = realloc(str_arr, str_arr_size * sizeof(char*));
    }
    substr = strtok(NULL, delim);
  }


  return str_arr;
}

char* str_concat(const char* str1, const char* str2) {
  const size_t length = (strlen(str1) + strlen(str2)) * sizeof(char) + 1;
  char* sum_str = malloc(length);

  snprintf(sum_str, length, "%s%s", str1, str2);

  return sum_str;
}

char* str_format(const size_t num, const char* str, ...) {
  va_list args;
  va_start(args, str);

  size_t str_size = 0;

  size_t buffer_capacity = strlen(str) > 256 ? strlen(str) : 256;

  char* buffer = malloc(buffer_capacity * sizeof(char));

  strcat(buffer, str);

  for (size_t i=0; i <num; i++) {
    
    char* arg = va_arg(args, char*);
    const size_t length = strlen(str);

    if (str_size + length >= buffer_capacity)  {
        buffer_capacity *= 2;
        buffer = realloc(buffer, buffer_capacity);
    }

    strcat(buffer, arg);
    str_size += length;
  }

  va_end(args);
  return buffer;
}


FileLoc compute_relative_position() {
  const Scanner s = lox.scanner;
  return (FileLoc) {
    .line  = s.line,
    .start = s.start - s.last_line,
    .end   = s.current - 1 - s.last_line,
  };
}

char* get_source_line(const size_t line_num) {
  char** arr = str_split(lox.scanner.source, "\n");
  return arr[line_num-1];
}
