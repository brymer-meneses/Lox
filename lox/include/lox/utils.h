#ifndef STR_UTILS_H
#define STR_UTILS_H

#include "stdlib.h"
#include "stdbool.h"
#include "lox/scanner.h"

bool is_real_number(const char* string);
bool is_boolean(const char* string);

char* substring(const char* string, const size_t begin, const size_t end);
char* read_file(const char* filename);
char* char_to_string(const char chr);

char** str_split(const char* str, const char* delim);

char* get_current_line();

#endif
