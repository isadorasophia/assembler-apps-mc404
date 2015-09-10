/***************************************************************
 * Description: Error reporting implementation
 * Author: Edson Borin (edson@ic.unicamp.br)
 ***************************************************************/
#include "error.h"
#include <stdlib.h>
#include <stdio.h>

/* Reports an error message. 
 * line is the input line number.
 * exit_code is the exit code. */ 
void report_error(char* msg, int line, int exit_code)
{
  fprintf(stderr, "ERROR on line %d\n", line);
  fprintf(stderr, "%s\n", msg);
  exit(exit_code);
}
