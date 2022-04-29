#ifndef __ARCH_H__
#define __ARCH_H__

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

int arch_init(void);

typedef void (*arch_load_out_cb)(char *line, void* context);
int arch_load(char* filename, arch_load_out_cb cb, void* context);

typedef uint16_t (*arch_save_cb)(char** line, void* context);
int arch_save(char* filename, arch_save_cb cb, void* context);

short arch_dir();

int arch_delete(char* filename);

#endif // __ARCH_H__
