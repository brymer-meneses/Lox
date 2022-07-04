#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define MAX_INPUT_LIMIT 1024
#define MAX_STRING_LIMIT 2147483647

#define INITIAL_TOKEN_ARRAY_SIZE 8

#include "stdbool.h"
#include "stdlib.h"
#include "tools/fileloc.h"
#include "token.h"

extern FileLoc FILE_LOC_NULL;
extern Token TOKEN_NULL;
extern LoxObject LOX_OBJECT_NULL;

#endif
