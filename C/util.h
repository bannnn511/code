#ifndef UTIL_H
#define UTIL_H
#include "errno.h"

#define handle_error(en, msg)                                                  \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)
#endif // UTIL_H
