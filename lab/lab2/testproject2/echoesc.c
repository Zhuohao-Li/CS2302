// minimal echo routine supporting ASCII escape characters
// They need to be enclosed in ""
// Beware! not much error checking (or comments for that matter)

// Necessary because most shells have builtin echo without escape
//  character translation. Solaris has an echo system utility that
//  does, Darwin (OSX) does not...

// This should emulate the printf user command
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
              
int main(int argc, char *argv[])
{
   int i = 1;
   char * arg;
   int c, number;
   
   while (i < argc) {
      arg = argv[i++];
      while (*arg) {
          if (*arg == '\\') {
              c = *(++arg);
              switch (c) {
                  case 0:                // oops
                      putchar('\\');
                      continue;
                  case '0':              // 2 digit octal number
                      number = 0;
                      if (!*++arg) {
                          putchar(number);
                          continue;
                      }
                      number = number * 8 + *arg - '0'; 
                      if (!*++arg) {
                          putchar(number);
                          continue;
                      }    
                      number = number * 8 + *arg - '0';
                      putchar(number);
                      break;
                  case 'a':               // alert character
                      putchar('\a');
                      break;
                  case 'b':               // backspace
                      putchar('\b');
                      break;                
                  case 'c':               // omit new line at end of line
                      break;              //     not supported
                  case 'f':               // form-feed
                      putchar('\f');
                      break;
                  case 'n':               // new-line
                      putchar('\n');
                      break;
                  case 'r':               // carriage return
                      putchar('\r');
                      break;
                  case 't':               // tab
                      putchar('\t');
                      break;                      
                  case 'v':               // vertical tab
                      putchar('\v');
                      break;
                  default:
                      putchar(c);
              }         
          } else {
              putchar(*arg);
          }
          arg++;
      }
      putchar(' ');                       // space between cmnd args
   }
   exit(0);
}
