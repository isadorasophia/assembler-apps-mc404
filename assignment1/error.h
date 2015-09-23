/* ********************************************************************* *
 * 
 *                                 Error
 * Description: responsible for reporting errors that occurred during
 * the execution.
 *
 * ********************************************************************* */
#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>

/* File error flag */
#ifndef FILE_ERROR
#define FILE_ERROR -1
#endif

/* Reports an error message. 
 *
 * line:            input line number (FILE_ERROR if none)
 * exit_code:       the exit code.
 * output:          where the message will be printed
 */
void report_error(FILE* output, const char* msg, int line, int exit_code);

#endif