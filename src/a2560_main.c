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

static int stricmp(const char *s1, const char *s2)
{
  unsigned char c1, c2;
  do {
    c1 = tolower(*s1);
    c2 = tolower(*s2);
    if (c1 < c2)
      return -1;
    else if (c1 > c2)
      return 1;
    s1++, s2++;
  } while (c1 != 0);
  return 0;
}


int out(int ch)
{
  putchar(ch);
  return 1;
}

int in(void)
{
  return getchar();
}

void print_copyright(void) {
  printf(
    "\n\nCopyright 2015-2016 Johan Van den Brande\n"
    "Copyright 2022 Jesus Garcia\n\n"
  );
}

void print_license(void) {
  print_copyright();
  printf(
    "Permission is hereby granted, free of charge, to any person obtaining a copy of this \n"
    "software and associated documentation files (the \"Software\"), to deal in the Software\n"
    "without restriction, including without limitation the rights to use, copy, modify, merge,\n"
    "publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons\n"
    "to whom the Software is furnished to do so, subject to the following conditions:\n\n"
    "The above copyright notice and this permission notice shall be included in all copies or\n"
    "substantial portions of the Software.\n\n"
    "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,\n"
    "INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR\n"
    "PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE\n"
    "FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR\n"
    "OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER\n"
    "DEALINGS IN THE SOFTWARE.\n\n");
}

void repl(void)
{
  sys_chan_ioctrl(0, 3, 0, 0);
  printf("Foenix/BASIC68K %s\n", BUILD_VER);
  printf("Type \"help\", \"copyright\", or \"license\" for more info.\n\n");
 
  char input[255];
  printf("] ");

  while ((sys_chan_readline(0, input, 255)) >= 0 )
  {
    printf("\n");
    if (stricmp(input, "bye") == 0) {
      break;
    } else if (stricmp(input, "help") == 0) {
    } else if (stricmp(input, "copyright") == 0) {
      print_copyright();
    } else if (stricmp(input, "license") == 0) {
      print_license();
    } else {
      basic_eval(input);
      
      if (evaluate_last_error()) {
        printf("ERROR: %s\n", evaluate_last_error());
        clear_last_error();
      }
    }
    
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
