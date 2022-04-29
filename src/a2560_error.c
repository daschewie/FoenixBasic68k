#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
// or inttypes.h

extern uint16_t __line;

const char *last_error = NULL;

void error(const char *error_msg)
{
  last_error = error_msg;
  if (__line > 0) {
    printf("\nERROR (LINE %d): %s\n\n", __line, error_msg);
  } else {
    printf("\nERROR: %s\n\n", error_msg);
  }
}
