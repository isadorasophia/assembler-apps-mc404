/* ********************************************************************* *
 * 
 *                               Data access
 * Description: responsible for the communication between the program
 * and the input and output.
 *
 * ********************************************************************* */
#ifndef DATA_ACCESS_H
#define DATA_ACCESS_H

#include <stdio.h>

/* File error flag */
#ifndef FILE_ERROR
#define FILE_ERROR -1
#endif

/* Safe buffer size */
#define BUFFER_SIZE 1024

/**
 * Default file data structure
 */
typedef struct File {
    int line;
    FILE* in;
    FILE* out;

    char buffer[BUFFER_SIZE];
} File;

/* File access */
void initialize_file (File*, const char*, const char*);
bool read_file (File*);
void write_map(MemMap*, FILE*);
void refresh_file (File*);
void clean_file (File*);

/* File handling */
void skip_line (File*);
void read_argument (File*, int);
ld read_constant(char* buffer, int line, FILE* output,
                 regex_t*, regex_t*);

#endif