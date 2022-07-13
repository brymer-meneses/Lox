#include "tools/fileloc.h"
#include "tools/utils.h"
#include "stdarg.h"
#include "limits.h"


static size_t max(size_t num1, size_t num2);
static size_t min(size_t num1, size_t num2);

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
    start = min(arg->start, start);
    end =   max(arg->end, end);
    line =  max(arg->line, line);
  }

  va_end(args);
  return fileloc_init(line, start, end);
}

static size_t min(size_t num1, size_t num2) {
  if (num1 < num2) {
    return num1;
  } 
  return num2;
}

static size_t max(size_t num1, size_t num2) {
  if (num1 > num2) {
    return num1;
  } 
  return num2;
}
