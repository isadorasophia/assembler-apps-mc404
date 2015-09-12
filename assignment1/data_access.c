/* ********************************************************************* *
 * 
 *                        Data access (implementation)
 * Description: responsible for the communication between the program
 * and the input and output.
 *
 * ********************************************************************* */

#include "data_structure.h"
#include "data_access.h"

#include "error.h"

/**
 * Initialize file data structure
 */
void initialize_file (File* f, const char* input, const char* output) {
    // if the output string is empty, set default output
    if (strlen(output) <= 0) {
        f->out = stdout;
    } else {
        f->out = fopen(input, "w");
    }

    // if the file could not be open
    if (f->out == NULL) {
        report_error(f->out, strcat(output, " could not be open."), FILE_ERROR, 1);
    }

    f->in = fopen(input, "r");

    // if the file could not be found
    if (f->in == NULL) {
        report_error(f->out, strcat(input, " could not be found."), FILE_ERROR, 1);
    }

    f->line = 1;
}

/**
 * Read a valid string from file and set to the buffer.
 * 
 * return:          if it was a successful read
 */
bool read_file (File* f) {
    char    garbage[GARBAGE_SIZE];      // garbage buffer
    char*   p,                          // helper
            c;

    // first, check file status
    c = fgetc(f->in);

    // get rid of skippable text
    while (c == ' ' || c == '\n' || c == '#') {
        // new line! just keep going
        if (c == '\n') {
            f->line++;

            c = fgetc(f->in);
        } 
        // comment line, just skip the entire line
        else if (c == '#') {
            fscanf(f->in, "%[^\n]", garbage);

            c = fgetc(f->in);
        } 
        // space, find next useful character
        else if (c == ' ') {
            c = fgetc(f->in);
        }
    }

    if (c == EOF) {
        // program has ended, go home
        return false;
    } else {
        // good to go, just pretend it never happened
        fseek(f->in, -1, SEEK_CUR);
    }

    fscanf(f->in, "%s", f->buffer);

    // detect if there was a hash symbol in the string
    p = strchr(f->buffer, '#');

    // if it was found, get rid of it and go to next line!
    if (p) {
        (*p) = '\0';

        fscanf(f->in, "%[^\n]", garbage);
    }

    return true;
}

/**
 * Restore a file from the beginning
 */
void refresh_file (File *f) {
    rewind(f->in);

    f->line = 1;
}

/**
 * Clean up
 */
void clean_file (File *f) {
    fclose(f->in);   
}