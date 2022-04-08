#include <stdio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#include "parser.h"

extern bool __RUNNING;
extern bool __STOPPED;

// static void
// sigint_handler(int signum)
// {
//   signal(SIGINT, sigint_handler);
//   if(__RUNNING){
//     __RUNNING = false;
//     __STOPPED = true;
//     printf("STOP\n");
//     fflush(stdout);
//   }
// }


int out(int ch)
{
  putchar(ch);
  return 1;
}

int in(void)
{
  return getchar();
}

void repl(void)
{
  printf("Foenix/BASIC68K %s\n", BUILD_VER);
  printf("Type \"help\", \"copyright\", or \"license\" for more info.\n");
 
  char input[255];
  printf("\n] ");
  while ((gets(input)) != NULL )
  {
    if (strcmp(input, "bye") == 0) {
      break;
    }
    
    basic_eval(input);
    
    if (evaluate_last_error()) {
      printf("ERROR: %s\n", evaluate_last_error());
      clear_last_error();
    }
    printf("\n] ");
  }

}

void run(char *file_name){
  FILE* file = fopen(file_name, "r");

  if (file == NULL) {
    fprintf(stderr, "Can't open %s\n", file_name);
    return;  
  }  

  char line[tokenizer_string_length];
  while (fgets(line, sizeof(line), file)) {
    if(line[strlen(line)-1]!='\n')
    {
      printf("ERROR: NO EOL\n");
      exit(1);      
    }
    basic_eval(line);
  }
  fclose(file);

  basic_run();
}

int main(int argc, char *argv[])
{
  basic_init(1024*8, 2048);
  basic_register_io(out, in);

  if (argc > 1){
    run(argv[1]);
  } else {  
    repl();
  }

  basic_destroy();

  return EXIT_SUCCESS;
}
