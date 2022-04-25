#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include "arch.h"
#include "mcp/syscalls.h"

#define	FA_READ				0x01
#define	FA_WRITE			0x02
#define	FA_OPEN_EXISTING	0x00
#define	FA_CREATE_NEW		0x04
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define	FA_OPEN_APPEND		0x30

int arch_init(void)
{
  // stub
  return 0;
}

  static char*
_get_path(void)
{
  return "/sd";
}

static void full_path(char *buffer, char *name, int size) {
    if (name[0] != '/') {
      sys_fsys_get_cwd(buffer, size);
      strcat(buffer, name);
    } else {
      strcpy(buffer, name);
    }

    char *suffix = strrchr(buffer, '.');
    if (suffix == NULL) {
      strcat(buffer, ".bas");
    }
}

int arch_load(char* name, arch_load_out_cb cb, void* context)
{
  char filename[256];
  full_path(filename, name, 256);

  int chan = sys_fsys_open(filename, FA_READ);
  if (chan < 0) {
    printf("Error: %d\n", chan);
    return 1;
  }

  char line[256];
  while(sys_chan_readline(chan, line, 256) > 0) {
    cb(line, context);
  }

  sys_fsys_close(chan);
  return 0;
}

int arch_save(char* name, arch_save_cb cb, void* context)
{
  char buffer[80];
  char filename[256];
  char* line;

  full_path(filename, name, 256);

  int chan = sys_fsys_open(filename, FA_WRITE | FA_CREATE_ALWAYS);
  if (chan < 0) {
    printf("Error: %d\n", chan);
    return 1;
  }

  for(;;){
    uint16_t number = cb(&line, context);
    if (line == NULL){
      break;
    }
    sprintf(buffer, "%d %s\n",number, line);
    sys_chan_write(chan, (unsigned char *)buffer, strlen(buffer));
  }
  sys_fsys_close(chan);

  return 0;
}

int arch_dir(arch_dir_out_cb cb, void* context)
{
  return 0;
}

int arch_delete(char* name){
  return 0;
}


void delay_ms(uint16_t count) {
  while(count--) {
    
  }
}