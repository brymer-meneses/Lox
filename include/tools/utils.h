#ifndef STR_UTILS_H
#define STR_UTILS_H

#include "stdlib.h"
#include "stdbool.h"
#include "lox/scanner.h"

bool is_real_number(const char* string);
bool is_boolean(const char* string);

char* substring(const char* string, const size_t begin, const size_t end);
char* read_file(const char* filename);

char** str_split(const char* str, const char* delim);
char*  str_concat(const char* str1, const char* str2);
char*  str_format(const char* str, ...);

#endif
