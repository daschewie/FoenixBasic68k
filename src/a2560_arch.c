#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include "arch.h"
#include "error.h"
#include "parser.h"
#include "mcp/syscalls.h"

#define	FA_READ				0x01
#define	FA_WRITE			0x02
#define	FA_OPEN_EXISTING	0x00
#define	FA_CREATE_NEW		0x04
#define	FA_CREATE_ALWAYS	0x08
#define	FA_OPEN_ALWAYS		0x10
#define	FA_OPEN_APPEND		0x30

/* File attribute bits for directory entry (FILINFO.fattrib) */
#define	AM_RDO	0x01	/* Read only */
#define	AM_HID	0x02	/* Hidden */
#define	AM_SYS	0x04	/* System */
#define AM_DIR	0x10	/* Directory */
#define AM_ARC	0x20	/* Archive */




int arch_init(void)
{
  unsigned int seed = sys_time_jiffies() % UINT_MAX;
  srand(seed);
  return 0;
}

static void full_path(char *buffer, char *name, int size) {
    strcpy(buffer, name);
    char *suffix = strrchr(buffer, '.');
    if (suffix == NULL) {
      strcat(buffer, ".bas");
    }
}

void arch_run_file(char* name)
{
  char filename[256];
  full_path(filename, name, 256);

  int chan = sys_fsys_open(filename, FA_READ);
  if (chan < 0) {
    printf("Can't open %s\n", filename);
    printf("Error: %d\n", chan);
    return;
  }

  char line[256];
  sys_chan_ioctrl(0, 0x03, 0, 0);
  while(sys_chan_readline(chan, (unsigned char *)line, 256) > 0) {
    if(line[strlen(line)-1]!='\n')
    {
      printf("ERROR: NO EOL\n");
      exit(1);      
    }
    basic_eval(line);
  }
  sys_fsys_close(chan);
  basic_run();
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
  while(sys_chan_readline(chan, (unsigned char *)line, 256) > 0) {
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

short arch_dir() {
    short res=0;
    short dir=0;
    t_file_info fno;

    char cwd[256];
    sys_fsys_get_cwd(cwd, 256);

    dir = sys_fsys_opendir(cwd);                       /* Open the directory */
    if (dir > 0) {
        for (;;) {
            res = sys_fsys_readdir(dir, &fno);                   /* Read a directory item */
            if (res != 0 || fno.name[0] == 0) break;  /* Break on error or end of dir */
            if (fno.attributes  & AM_DIR) {                    /* It is a directory */
                printf("%s/\n", fno.name);
            } else {                                       /* It is a file. */
                printf("%s\n", fno.name);
            }
        }
        sys_fsys_closedir(dir);
    }

    return 0;
}
      //  while (entry != 0) {
      //       if (entry->info.size < 1024) {
      //           printf("%-20.20s %d B\n", entry->info.name, (int)entry->info.size);
      //       } else if (my_file.size < 1024*1024) {
      //           printf("%-20.20s %d KB\n", entry->info.name, (int)entry->info.size / 1024);
      //       } else {
      //           printf("%-29.20s %d MB\n", entry->info.name, (int)entry->info.size / (1024*1024));
      //       }

int arch_delete(char* name) {
  if(sys_fsys_delete(name) < 0) {
    printf("Error: Deleting %s", name);
  }
  return 0;
}

void arch_open_file(open_file *file) {
  int mode = 0;
  switch (file->mode)
  {
  case MODE_OUTPUT:
    mode = FA_CREATE_ALWAYS | FA_WRITE;
    break;

  case MODE_INPUT:
    mode = FA_READ;
    break;
  
  default:
    printf("Unsupported MODE\n");
    break;
  }

  file->channel = sys_fsys_open(file->name, mode);
  if (file->channel >= 0) {
    file->is_open = true;
  } else {
    file->channel = 0;
    file->is_open = false;
  }
}

void arch_close_file(open_file *file) {
  if (file->is_open) {
    sys_fsys_close(file->channel);
    file->channel = 0;
    file->is_open = false;
  }
}

int arch_eof(open_file *file) {
  return 0;
}

size_t arch_lof(open_file *file) {
  return 0;
}

void arch_writeln(open_file *file, const char *str, bool newline) {
  sys_chan_write(file->channel, (const unsigned char *) str, strlen(str));
  if (newline) sys_chan_write_b(file->channel, (unsigned char) '\n');
}

void arch_bload(char *filename, uint32_t address) {
  short rc = sys_fsys_load(filename, address, 0);
  if (rc < 0) {
    error("UNABLE TO LOAD BINARY");
  }
}


void delay_ms(uint16_t count) {
  while(count--) {
    
  }
}

int strnicmp(const char* s1, const char* s2, size_t n) {

  if (n == 0)
    return 0;

  do {
    if (tolower((unsigned char) *s1) != tolower((unsigned char) *s2++))
      return (int)tolower((unsigned char)*s1) -
	(int) tolower((unsigned char) *--s2);
    if (*s1++ == 0)
      break;
  } while (--n != 0);

  return 0;
}