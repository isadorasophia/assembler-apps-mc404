/* *********************************************************************** *
 *            .                                                            *
 *           /'                                                            *
 *       .  //                                                             *  
 *       |\//7                                                             *
 *      /' " \                                                             *
 *     .   . .                                                             *
 *     | (    \     '._                                                    *
 *     |  '._  '    '. '                                                   *
 *     /    \'-'_---. ) )              MC404 - First assignment            *
 *    .              :.'     Description: An assembler that translates IAS *
 *    |               \      languague into machine languague              *
 *    | .    .   .     .                                                   *
 *    ' .    |  |      |     Author: Isadora Sophia Garcia Rodopoulos      *
 *     \^   /_-':     /      RA: 158018                                    *
 *     / | |    '\  .'                                                     *
 *    / /| |     \\  |                                                     *
 *    \ \( )     // /                                                      *
 *     \ | |    // /                                                       *
 *      L! !   // /                                                        *
 *       [_]  L[_|                                                         *
 *                                                                         *
 * *********************************************************************** */

/**
 * Libraries
 */
#include "data_structure.h"
#include "data_access.h"
#include "linkedlist.h"
#include "error.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regex.h>                      // regular expressions
#include <search.h>                     // enables binary search tree

int main (int argc, char *argv[]) {
    File        file;

    MemMap      map[MAX_WORDS];         // final memory map
    Position    current_pos;            // keep in track of current position
    Model       current_model;          // helps to identify the line model

    Node*       cur_labels   = NULL;    // labels to be set

    void*       t_labels     = NULL;    // total labels so far
    void*       t_sym        = NULL;    // total sym so far

    void*       instructions = NULL;    // dictionary of instructions

    regex_t     label_regex, sym_regex,
                instr_regex, directive_regex,
                hex_regex, decimal_regex;

    int         value,
                current_line = 0;

    /**
     * Initialize data!
     */
    /* Set file ready according to the arguments received */
    if (argc < 2) {
        report_error(stdout, "Please, specify the input file.", FILE_ERROR, 1);
    } else if (argc == 2) {
        initialize_file(&file, argv[1], "");
    } else if (argc == 3) {
        initialize_file(&file, argv[1], argv[2]);
    }

    /* Set each regex by its pattern */
    regcomp(&label_regex, LABEL_REGEX, REG_EXTENDED);
    regcomp(&sym_regex, SYM_REGEX, REG_EXTENDED);

    regcomp(&instr_regex, INSTR_REGEX, REG_EXTENDED);
    regcomp(&directive_regex, DIRECTIVE_REGEX, REG_EXTENDED);

    regcomp(&hex_regex, HEX_REGEX, REG_EXTENDED);
    regcomp(&decimal_regex, DEC_REGEX, REG_EXTENDED);

    /* Set map */
    initialize_mem(map);

    /* Set instructions */
    initialize_instr(&instructions);

    /**
     * Start working!
     */
    while (read_file(&file)) {
        if (current_line != file.line) {
            // new line, set the expected input
            current_model = LABEL;
        }

        /* Label */
        if (match(&label_regex, file.buffer)) {
            // check if the label was placed correctly
            if (current_model == LABEL) {
                current_model = INSTR_OR_DIR;

                clean_constraints(file.buffer, false);

                // add to the pile
                push(&cur_labels, file.buffer);
            } 
            // if the label was placed incorrectly, report an error
            else {
                report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
            }
        }

        /* Instruction */
        else if (match(&instr_regex, file.buffer)) {
            if (find_label(file.buffer, &instructions, &value)) {
                if (current_model != END) {

                    /* Get the proper argument */
                    if (read_file(&file) && current_line == file.line) {
                        /* Sym or label reference */
                        if (match(&sym_regex, file.buffer)) {
                            if (find_label(file.buffer, &instructions, &value)) {
                                // found
                            } else {
                                // if a valid mnemonic was not found, report an error
                                report_error(file.out, strcat(file.buffer, " is not a valid mnemonic!"), file.line, 1);
                            }
                        }

                        /* Hex */
                        else if (match(&hex_regex, file.buffer)) {
                            if (find_label(file.buffer, &instructions, &value)) {
                                // found
                            } else {
                                // if a valid mnemonic was not found, report an error
                                report_error(file.out, strcat(file.buffer, " is not a valid mnemonic!"), file.line, 1);
                            }
                        }

                        /* Decimal */
                        else if (match(&decimal_regex, file.buffer)) {
                            if (find_label(file.buffer, &instructions, &value)) {
                                // found
                            } else {
                                // if a valid mnemonic was not found, report an error
                                report_error(file.out, strcat(file.buffer, " is not a valid mnemonic!"), file.line, 1);
                            }
                        }

                        else {
                            report_error(file.out, strcat(file.buffer, " is not a valid argument!"), file.line, 1);
                        }

                        current_model = END;
                    } else {
                        report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                    }

                } else {
                    report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                }
            } else {
                // if a valid mnemonic was not found, report an error
                report_error(file.out, strcat(file.buffer, " is not a valid mnemonic!"), file.line, 1);
            }
        }

        /* Directive */
        else if (match(&directive_regex, file.buffer)) {
            if (find_label(file.buffer, &instructions, &value)) {
                if (current_model != END) {

                    // deal with it

                    current_model = END;
                } else {
                    report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                }
            } else {
                // if a valid mnemonic was not found, report an error
                report_error(file.out, strcat(file.buffer, " is not a valid directive!"), file.line, 1);
            }
        }

        else {
            // no pattern found
            report_error(file.out, strcat(file.buffer, " is not a valid element!"), file.line, 1);
        }
    }

    /**
     * Clean up the mess...
     */
    regfree(&label_regex);
    regfree(&sym_regex);

    regfree(&instr_regex);
    regfree(&directive_regex);

    regfree(&hex_regex);
    regfree(&decimal_regex);

    clean_file(&file);

    clean_list(&cur_labels);

    return 0;
}
