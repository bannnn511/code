#include <stdio.h>
#include "errno.h"

#define handle_error(en, msg)                                                  \
  do {                                                                         \
    errno = en;                                                                \
    perror(msg);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)