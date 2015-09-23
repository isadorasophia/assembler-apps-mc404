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

/* ---------------------------------------------------------------------
 * Definitions
 * --------------------------------------------------------------------- */
/* File error flag */
#ifndef FILE_ERROR
#define FILE_ERROR -1
#endif

/* Safe buffer size */
#define BUFFER_SIZE 1024

/* ---------------------------------------------------------------------
 * General structures
 * --------------------------------------------------------------------- */
/**
 * Default file data structure
 */
typedef struct File {
    int line;
    FILE* in;
    FILE* out;

    char buffer[BUFFER_SIZE];
} File;

/* ---------------------------------------------------------------------
 * File access functions
 * --------------------------------------------------------------------- */
/* Initialize file data structure */
void initialize_file (File*, char*, char*);

/* Read a valid string from file and set to the buffer. */
bool read_file (File*);

/* Write map into file */
void write_map(MemMap*, FILE*);

/* Restore a file from the beginning */
void refresh_file (File*);

/* Clean up */
void clean_file (File*);

/* ---------------------------------------------------------------------
 * File handling functions
 * --------------------------------------------------------------------- */
/* Skip to the newline character of a sentence */
void skip_line (File*);

/* Read an argument and make the appropriate check */
void read_argument (File*, int);

/* Read an argument converted in string, either in hex or decimal */
ld read_constant(char* buffer, int line, FILE* output,
                 regex_t*, regex_t*);

#endif