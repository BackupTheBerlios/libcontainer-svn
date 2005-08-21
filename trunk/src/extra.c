#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <extra.h>

#define ERR_LOCATION stderr

void extra_warn(char *fmt, ...) {
  va_list args;

  /* initial tagging */
  fprintf(ERR_LOCATION, "libcontainer: ");

  /* now the rest of the user data */
  va_start(args, fmt);
  vfprintf(ERR_LOCATION, fmt, args);
  va_end(args);
}