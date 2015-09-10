#include "data_structure.h"
#include "data_access.h"

/**
 * Initialize file data structure
 */
void initializeFile (File* f, const char* filename) {
    f->f = fopen(filename, "r");

    f->line = 1;
}

/**
 * Read from file and set to the buffer.
 * 
 * return:          if it was a succesful read
 */
bool readFile (File* f) {
    // check file status
    int c = fgetc(f->f);

    // new line! just keep going
    while (c == '\n') {
        f->line++;

        c = fgetc(f->f);
    }

    if (c == EOF) {
        // program has ended
        return false;
    } else {
        // nothing new, just pretend it never happened
        fseek(f->f, -1, SEEK_CUR);
    }

    fscanf(f->f, "%s", f->buffer);

    return true;
}

/**
 * Restore a file from the beginning
 */
void refreshFile (File *f) {
    rewind(f->f);

    f->line = 1;
}

/**
 * Clean up
 */
void cleanFile (File *f) {
    fclose(f->f);   
}