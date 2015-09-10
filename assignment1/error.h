/***************************************************************
 * Description: Error reporting API
 * Author: Edson Borin (edson@ic.unicamp.br)
 ***************************************************************/
#ifndef _ERROR_H
#define _ERROR_H

/* Reports an error message. 
 * line is the input line number.
 * exit_code is the exit code. */ 
void report_error(char* msg, int line, int exit_code);

#endif
