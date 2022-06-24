#ifndef STR_UTILS_H
#define STR_UTILS_H

#include "stdlib.h"
#include "stdbool.h"

char* substring(const char* string, const size_t begin, const size_t end);
bool is_real_number(const char* string);
bool is_boolean(const char* string);
char* read_file(const char* filename);

#endif
