#include <stdio.h>              // file data structures

/**
 * Default file data structure
 */
typedef struct File {
    int line;
    FILE* f;

    char buffer[1000];
} File;

/* File access */
void initializeFile (File* f, const char* filename);
bool readFile (File* f);
void refreshFile (File *f);
void cleanFile (File *f);