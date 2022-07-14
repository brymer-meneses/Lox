#include "tools/fileloc.h"
#include "tools/utils.h"
#include "stdarg.h"
#include "limits.h"
#include <assert.h>

FileLoc* fileloc_init(const size_t line, const size_t start, const size_t end) {
  FileLoc* fl = malloc(1 * sizeof(FileLoc));
  fl->line = line;
  fl->start = start;
  fl->end = end;
  return fl;
}

FileLoc* fileloc_range(const unsigned int num_args, ...) {

  va_list args;
  va_start(args, num_args);

  size_t start = ULONG_MAX;
  size_t end = 0;
  size_t line = 1;

  for (unsigned int i=0; i<num_args; i++) {
    FileLoc* arg = va_arg(args, FileLoc*);
    assert(arg != NULL);

    start =   arg->start < start ? arg->start : start;
    end   =   arg->end   > end   ? arg->end   : end;
    line  =   arg->line  > line  ? arg->line  : line;
  }

  va_end(args);
  return fileloc_init(line, start, end);
}

