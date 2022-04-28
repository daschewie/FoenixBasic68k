#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mcp/syscalls.h"

#include "io.h"

extern basic_putchar __putch;
extern basic_getchar __getch;

  void
basic_io_print(char* buffer)
{
  sys_chan_write(0, buffer, strlen(buffer));
}  

char*
basic_io_readline(char* prompt, char* buffer, size_t buffer_size)
{
  basic_io_print(prompt);
  sys_chan_readline(0, buffer, buffer_size);
  basic_io_print("\n");
  return buffer;
}

