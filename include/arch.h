#ifndef __ARCH_H__
#define __ARCH_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
  MODE_INPUT,
  MODE_OUTPUT,
  MODE_RANDOM
} open_file_mode;

typedef struct {
  bool is_open;
  char name[129];
  short channel;
  open_file_mode mode;
} open_file;

int arch_init(void);

typedef void (*arch_load_out_cb)(char *line, void* context);
int arch_load(char* filename, arch_load_out_cb cb, void* context);

typedef uint16_t (*arch_save_cb)(char** line, void* context);
int arch_save(char* filename, arch_save_cb cb, void* context);

short arch_dir();

int arch_delete(char* filename);

void arch_run_file(char* name);

void arch_open_file(open_file *file);
void arch_close_file(open_file *file);
int arch_eof(open_file *file);
size_t arch_lof(open_file *file);


#endif // __ARCH_H__
