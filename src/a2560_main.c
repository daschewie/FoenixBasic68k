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
  printf("BASIC68K 0.1.0 (main, Apr 8 2022, 15:00) [cc68k %s.%s.%s]\n",
    __CALYPSI_VERSION_MAJOR__,
    __CALYPSI_VERSION_MINOR__,
    __CALYPSI_VERSION_FIX_LEVEL__
  );
  printf("Type \"help\", \"copyright\", or \"license\" for more info.\n\n");
 
  char input[255];
  printf("] ");
  while ((gets(input)) != NULL )
  {
    if (strcmp(input, "QUIT") == 0) {
      memset(input, 0, 255);
      break;
    }
    
    basic_eval(input);
    
    if (evaluate_last_error()) {
      printf("ERROR: %s\n", evaluate_last_error());
      clear_last_error();
    }
    memset(input, 0, 255);
    printf("] ");
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
