#include "arch.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

int arch_load(char* name, arch_load_out_cb cb, void* context)
{
  char* filename;
  asprintf(&filename, "%s/%s.bas", _get_path(), name);
  FILE* fp = fopen(filename, "r");
  if(!fp){
    return 1;
  }
  char line[256];
  while(fgets(line, 256, fp) != NULL) {
    cb(line, context);
  }
  fclose(fp);
  free(filename);
  return 0;
}

int arch_save(char* name, arch_save_cb cb, void* context)
{
  char* line;
  char* filename;
  asprintf(&filename, "%s/%s.bas", _get_path(), name);
  FILE* fp = fopen(filename, "w"); 
  if(!fp){
    return 1;
  }
  for(;;){
    uint16_t number = cb(&line, context);
    if (line == NULL){
      break;
    }
    fprintf(fp, "%d %s\n",number, line);
  }
  fclose(fp);

  free(filename);

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