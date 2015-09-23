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
void report_error(FILE* output, const char* msg, int line, int exit_code)
{
    if (line != FILE_ERROR) {
        fprintf(output, "ERROR on line %d\n", line);
    }

    fprintf(output, "%s\n", msg);
    exit(exit_code);
}
