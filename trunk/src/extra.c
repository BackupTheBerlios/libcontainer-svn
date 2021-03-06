#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define ERR_LOCATION stderr

void libcontainer_warn(const char *file, int line, char *fmt, ...) {
  va_list args;

  /* initial tagging */
  fprintf(ERR_LOCATION, "libcontainer %s [ %d ]: ", file, line);

  /* now the rest of the user data */
  va_start(args, fmt);
  vfprintf(ERR_LOCATION, fmt, args);
  va_end(args);
}
