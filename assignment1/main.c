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
    Position    cur_pos;                // keep in track of current position
    Model       cur_model;              // helps to identify the line model

    Node*       cur_labels   = NULL;    // labels to be set

    void*       t_labels     = NULL;    // total labels so far
    void*       t_sym        = NULL;    // total sym so far

    void*       instructions = NULL;    // dictionary of instructions

    int         value,
                cur_line = 0,
                tmp_int;

    char        tmp_str[65];            // helper variable for strings

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

    /* Initialize memory */
    cur_pos.address = 0;
    cur_pos.state = left;

    /**
     * First step:
     *  1) Attribute all labels and sym
     *  2) Check if commands are correctly placed in-line
     *  3) Check if memory allocation is aligned
     */
    while (read_file(&file)) {
        /* Set line */
        if (cur_line != file.line) {
            // new line, set the expected input
            cur_model = LABEL;
        }

        /* Label */
        if (match(&label_regex, file.buffer)) {
            // check if the label was placed correctly
            if (cur_model == LABEL) {
                cur_model = INSTR_OR_DIR;

                // get rid of the constraints
                clean_constraints(file.buffer, false);

                // add to the pile
                push(&cur_labels, file.buffer);
            } 

            // if the label was placed incorrectly
            else {
                report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
            }
        }

        /* Instruction */
        else if (match(&instr_regex, file.buffer)) {
            // check if the instruction was placed correctly
            if (cur_model != END) {
                cur_model = END;

                /* occupies memory! 
                 * 2nd step will take care of it, just set all pending
                 * labels to current position of memory */
                set_labels(&cur_labels, &cur_labels, 
                        cur_pos.address, file.line);

                // proceed to next available position
                update_position(&cur_pos, GO_NEXT);
            }

            // if the instruction was placed incorrectly
            else {
                report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
            }
        }

        /* Directive */
        else if (match(&directive_regex, file.buffer)) {
            // check if the instruction was placed correctly
            if (cur_model != END) {
                cur_model = END;

                /* Check which directive was called and deal with it 
                 * accordingly */
                // .set
                if (strcmp(file.buffer, ".set" == 0) {

                    /* 1st argument! */
                    if (read_file(&file) && cur_line == file.line) {
                        // is it a valid sym?
                        if (match(&sym_regex, file.buffer)) {
                            // ok, save it!
                            strcpy (tmp_str, file.buffer);

                            /* 2nd argument! */
                            if (read_file(&file) && cur_line == file.line) {
                                // read value
                                tmp_int = read_constant(file.buffer, 
                                        file.line);

                                /* assuming everything worked fine, 
                                 * insert in map */
                                insert_str(tmp_str, tmp_int, &t_sym);
                            }

                            // if the 2nd argument was declared incorrectly
                            else {
                                report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                            }

                        }

                        // invalid sym
                        else {
                            report_error(file.out, strcat(file.buffer, " is not a valid SYM!"), file.line, 1);
                        }
                    }

                    // if the 1st argument was declared incorrectly
                    else {
                        report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                    }

                }

                // .org
                else if (strcmp(file.buffer, ".org" == 0)) {

                    /* 1st argument! */
                    if (read_file(&file) && cur_line == file.line) {
                        // read value
                        tmp_int = read_constant(file.buffer, file.line);

                        /* assuming everything worked fine, 
                         * go to position */
                        update_position(&position, tmp_int);
                    }

                    // if the 1st argument was declared incorrectly
                    else {
                        report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                    }

                } 

                // .align
                else if (strcmp(file.buffer, ".align" == 0)) {

                    /* 1st argument! */
                    if (read_file(&file) && cur_line == file.line) {
                        // read value
                        tmp_int = read_constant(file.buffer, file.line);

                        // set min memory it can occupy as aligned
                        if (cur_pos.state == right) {
                            cur_pos.address++;
                        }

                        // find min multiple above current address
                        tmp_int = min_mul(tmp_int, cur_pos.address);

                        // proceed to position
                        update_position(&cur_pos, tmp_int);
                    }

                    // if the 1st argument was declared incorrectly
                    else {
                        report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                    }

                } 

                // .wfill
                else if (strcmp(file.buffer, ".wfill" == 0)) {

                    /* 1st argument! */
                    if (read_file(&file) && cur_line == file.line) {
                        // read value
                        tmp_int = read_constant(file.buffer, file.line);

                        /* 2nd argument! */
                        if (read_file(&file) && cur_line == file.line) {
                            /* occupies memory!
                             * 2nd step will take care of it, just set all 
                             * pending labels to current position of memory */
                            set_labels(&cur_labels, &cur_labels, 
                                    cur_pos.address, file.line);

                            // proceed to next available position after fill
                            update_position(&cur_pos, cur_pos.address + tmp_int);
                        }

                        // if the 2nd argument was declared incorrectly
                        else {
                            report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                        }

                    }

                    // if the 1st argument was declared incorrectly
                    else {
                        report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                    }

                }

                // .word
                else if (strcmp(file.buffer, ".word" == 0)) {
                    // does it occupy only a 20-bit command?
                    if (cur_pos.state == right) {
                        report_error(file.out, strcat(file.buffer, " is declared in non-aligned word!"), file.line, 1);
                    }

                    /* occupies memory!
                     * 2nd step will take care of it, just set all pending
                     * labels to current position of memory */
                    set_labels(&cur_labels, &cur_labels, 
                            cur_pos.address, file.line);

                    // proceed to next available position after 40-bit fill
                    update_position(&cur_pos, cur_pos.address + 2);
                }

                // if a valid directive was not found
                else {
                    report_error(file.out, strcat(file.buffer, " is not a valid directive!"), file.line, 1);
                }
            }

            // if the directive was placed incorrectly 
            else {
                report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
            }
        }

        /* None of the patterns matched! */
        else {
            report_error(file.out, strcat(file.buffer, " is not a valid element!"), file.line, 1);
        }
    }

    /* 1st step is finished, restart structures! */
    refresh_file(&file);

    cur_pos.address = 0;
    cur_pos.state = left;

    /**
     * Second step:
     *  1) Actually set the final file
     */
    while (read_file(&file)) {
        /* Label */
        if (match(&label_regex, file.buffer)) {
            // was already dealt in 1st step!
            continue
        }

        /* Instruction */
        else if (match(&instr_regex, file.buffer)) {
            // is it a valid instruction?
            if (find_label(file.buffer, &instructions, &value)) {
                // ok, save it!
                tmp_int = value;

                /* Get the argument! */
                if (read_file(&file) && cur_line == file.line) {
                    // get rid of the constraints
                    clean_constraints(file.buffer, 0);

                    /* Sym or label reference */
                    if (match(&sym_regex, file.buffer)) {
                        if (find_label(file.buffer, &t_syms, &value) || find_label(file.buffer, &t_labels, &value)) {

                            tmp_int = value;
                        }

                        // this is an inexistent sym/label!
                        else {
                            report_error(file.out, strcat(file.buffer, " is not a valid argument!"), file.line, 1);
                        }
                    }

                    /* Hex or decimal reference */
                    else {
                        tmp_int = read_constant(file.buffer, file.line);
                    }

                    // saves the instruction
                    sprintf(map[cur_pos.address].content[cur_pos.state], "%02X %03X", tmp_int, tmp_int);

                    // proceed to next available position
                    update_position(&cur_pos, GO_NEXT);
                } 

                // if the argument was declared in a new line 
                else {
                    report_error(file.out, strcat(file.buffer, " is placed incorrectly!"), file.line, 1);
                }

            }

            // if a valid mnemonic was not found 
            else {
                report_error(file.out, strcat(file.buffer, " is not a valid mnemonic!"), file.line, 1);
            }
        }

        /* Directive */
        else if (match(&directive_regex, file.buffer)) {
            if (find_label(file.buffer, &instructions, &value)) {
                // deal
            } else {
                // if a valid mnemonic was not found, report an error
                report_error(file.out, strcat(file.buffer, " is not a valid directive!"), file.line, 1);
            }
        }

        /* None of the patterns matched! 
         * (should already have been taken care of, but just in case) */
        else {
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
