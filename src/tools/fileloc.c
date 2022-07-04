#include "tools/fileloc.h"
#include "tools/utils.h"
#include "stdarg.h"
#include "limits.h"

static size_t min(size_t num1, size_t num2) {
  if (num1 < num2) {
    return num1;
  } 
  return num2;
}


FileLoc fileloc_init(const size_t line, const size_t start, const size_t end) {
  return (FileLoc) {
    .line = line,
    .start = start,
    .end = end,
  };
}

FileLoc fileloc_range(const unsigned int num_args, ...) {

  va_list args;
  va_start(args, num_args);

  size_t start = ULONG_MAX;
  size_t end = ULONG_MAX;
  size_t line = ULONG_MAX;

  for (unsigned int i=0; i<num_args; i++) {
    FileLoc arg = va_arg(args, FileLoc);
    start = min(arg.start, start);
    end = min(arg.end, end);
    line = min(arg.line, line);
  }

  va_end(args);
  return (FileLoc) {
    .start = start,
    .end = end,
    .line = line,
  };
}
