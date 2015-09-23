/* ********************************************************************* *
 * 
 *                        Data access (implementation)
 * Description: responsible for the communication between the program
 * and the input and output.
 *
 * ********************************************************************* */

#include <string.h>
#include <stdlib.h>

#include "data_structure.h"
#include "data_access.h"
#include "error.h"

/**
 * Initialize file data structure
 */
void initialize_file(File* f, char* input, char* output) {
    // if the output string is empty, set default output
    if (strlen(output) <= 0) {
        f->out = stdout;
    } else {
        f->out = fopen(output, "w");
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
bool read_file(File* f) {
    char* p, c;

    // first, check file status
    c = fgetc(f->in);

    // get rid of skippable text
    while (c == ' ' || c == '\n' || c == '#') {
        // new line! just keep going
        if (c == '\n') {
            f->line++;

            c = fgetc(f->in);
        } else if (c == '#') {
            // comment line, just skip the entire line
            skip_line(f);

            c = fgetc(f->in);
        } else if (c == ' ') {
            // space, find next useful character
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

        skip_line(f);
    }

    return true;
}

/**
 * Write map into file
 */
void write_map(MemMap* map, FILE* output) {
    int i;

    for (i = 0; i < MAX_WORDS; i++) {
        if (map[i].used == true) {
            fprintf(output, "%.3X %s %s\n", i, map[i].content[left], 
                    map[i].content[right]);
        }
    }
}

/**
 * Restore a file from the beginning
 */
void refresh_file(File *f) {
    rewind(f->in);

    f->line = 1;
}

/**
 * Clean up
 */
void clean_file(File *f) {
    fclose(f->in);   
}

/* ---------------------------------------------------------------------
 * File handling functions
 * --------------------------------------------------------------------- */
/**
 * Skip to the newline character of a sentence
 */
void skip_line (File* f) {
    char t = 0;

    // go until it finds the newline
    do {
        t = fgetc(f->in);
    } while (t != '\n' && t != EOF);

    // good to go, just leave it set!
    fseek(f->in, -1, SEEK_CUR);
}

/**
 * Read an argument and make the appropriate check
 *
 * cur_line:        line number, SKIP to ignore
 */
void read_argument (File* f, int cur_line) {
    // check if an error occurred
    if (!read_file(f) || 
        (cur_line != SKIP && cur_line != f->line)) {
        report_error(f->out, strcat(f->buffer, " is placed incorrectly!"), 
                     f->line, 1);
    }
}

/** 
 * Read an argument converted in string, either in hex or decimal.
 * Report an error if it wasn't declared correctly.
 *
 * return:          its value in decimal 
 */
ld read_constant(char* buffer, int line, FILE* output,
                 regex_t* decimal_regex, regex_t* hex_regex) {
    ld tmp_ld;

    if (match(decimal_regex, buffer)) {
        // save value
        tmp_ld = atol(buffer);
    } else if (match(hex_regex, buffer)) {
        // save value as decimal
        sscanf(buffer, "%lx", &tmp_ld);
    } else {
        // none of them, report an error!
        report_error(output, strcat(buffer, " is not a valid argument!"), line, 1);
    }

    return tmp_ld;
}