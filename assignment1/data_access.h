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

/* Maximum size of useless string */
#define GARBAGE_SIZE 512

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
void initialize_file (File* f, const char* input, const char* output);
bool read_file (File* f);
void refresh_file (File *f);
void clean_file (File *f);

#endif