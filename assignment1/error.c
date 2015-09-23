/* ***********************************************************************
 * 
 *                         Error (implementation)
 * Description: responsible for reporting errors that occurred during
 * the execution.
 *
 * ********************************************************************* */
#include "error.h"

/* Reports an error message. 
 * line is the input line number.
 * exit_code is the exit code. */ 
void report_error(const char* msg, int line, int exit_code)
{
    if (line != FILE_ERROR) {
        fprintf(stderr, "ERROR on line %d\n", line);
    }

    fprintf(stderr, "%s\n", msg);
    exit(exit_code);
}
