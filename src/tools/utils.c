#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "assert.h"
#include "ctype.h"

#include "tools/utils.h"
#include "lox/scanner.h"
#include "lox/filelocation.h"
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

  for (int i=0; i<string_length; i++) {
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

char* char_to_string(const char chr) {
  char* string = malloc(2*sizeof(char));
  sprintf(string, "%c", chr);
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

FileLoc fileloc_init(const size_t line, const size_t start, const size_t end) {
  return (FileLoc) {
    .line = line,
    .start = start,
    .end = end,
  };
}

FileLoc compute_relative_position() {
  const Scanner s = lox.scanner;
  return (FileLoc) {
    .line  = s.line,
    .start = s.start - s.last_line,
    .end   = s.current - 1 - s.last_line,
  };
}

char* get_current_line() {
  char** arr = str_split(lox.scanner.source, "\n");
  return arr[0];
}
