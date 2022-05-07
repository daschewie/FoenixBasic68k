#include <stdio.h>
#include <math.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <ctype.h>

#include "parser.h"
#include "line_edit.h"
#include "arch.h"

extern bool __RUNNING;
extern bool __STOPPED;

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

void print_help() {
  printf(
    "\n"
    "Interactive Commands\n"
    "====================\n"
    "HELP            : Displays this screen\n"
    "COPYRIGHT       : Displays copyright notice\n"
    "LICENSE         : Displays end user LICENSE\n"
    "BYE             : Exits BASIC\n"
    "NEW             : Clears PROGRAM from memory\n"
    "CLEAR           : Clears VARIABLES from memory\n"
    "DIR             : Displays listing of current directory\n"
    "CHDIR \"path\"    : Changes current directory\n"
    "LOAD \"filename\" : Loads a PROGRAM from disk\n"
    "SAVE \"filename\" : Saves current PROGRAM to disk\n"
    "LIST            : Displays listing of current PROGRAM\n"
    "RUN             : Executes current PROGRAM\n"
    "\n"
    "Useful Links\n"
    "============\n"
    "Source Code     : https://github.com/daschewie/basic68k\n"
    "Releases        : https://github.com/daschewie/basic68k/releases\n"
    "Documentation   : https://foenix.dev/\n"
    "\n"
    "Credits\n"
    "=======\n"
    "Jesus Garcia         : Code Monkey & Maintainer\n"
    "Johan Van den Brande : BASIC Core\n"
    "Peter Weingar        : Line Editor routines\n"
    "H\x86kan Th\x94rngren      : Calypsi C Compiler\n"
    "Stefany Allaire      : Foenix Hardware\n"
    "\n"
  );
}

void repl(void)
{
  printf("Foenix/BASIC68K %s\n", BUILD_VER);
  printf("Type \"help\", \"copyright\", or \"license\" for more info.\n\n");
 
  char input[128];
  printf("] ");

  while (1)
  {
    short rc = cli_readline(0, input);
    switch(rc) {
      case -1:
        // ctx switch
        break;
      case -2:
        // help
        print_help();
        printf("] ");
        break;
      default:
        printf("\n");
        if (stricmp(input, "bye") == 0) {
          return;
        } else if (stricmp(input, "help") == 0) {
          print_help();
        } else if (stricmp(input, "copyright") == 0) {
          print_copyright();
        } else if (stricmp(input, "license") == 0) {
          print_license();
        } else {
          basic_eval(input);
        }
        
        printf("] ");
    }
  }
}

int main(int argc, char *argv[])
{
  basic_init(1024*8, 2048);
  basic_register_io(out, in);

  if (argc > 1){
    arch_run_file(argv[1]);
  } else {  
    repl();
  }

  basic_destroy();

  return EXIT_SUCCESS;
}
