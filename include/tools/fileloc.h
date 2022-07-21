#ifndef FILE_LOCATION_H
#define FILE_LOCATION_H

#include "stdlib.h"

typedef struct {
   size_t line;
   size_t start;
   size_t end;
} FileLoc;

FileLoc* fileloc__init(const size_t line, const size_t start, const size_t end);
FileLoc* fileloc__range(const unsigned int num_args, ...);

#endif
