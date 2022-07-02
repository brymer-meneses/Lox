#ifndef FILE_LOCATION_H
#define FILE_LOCATION_H

#include "stdlib.h"
#include "declarations.h"


FileLoc fileloc_init(const size_t line, const size_t start, const size_t end);

#endif
