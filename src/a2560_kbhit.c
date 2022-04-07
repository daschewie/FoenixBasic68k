#include <stdbool.h>
#include <stdio.h>
#include "mcp/syscalls.h"

int kbhit(void) {
  int status = sys_chan_status(0);
  return (status & CDEV_STAT_READABLE);
}
