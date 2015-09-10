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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <regex.h>              // regular expressions
#include <search.h>             // enables binary search tree

#include "data_structure.h"
#include "linkedlist.h"
#include "error.h"
#include "data_access.h"

int main () {
    regex_t label, instr, directive,
            hex, decimal;

    /* Set each regex by its pattern */
    regcomp(&label, "^[_a-zA-Z][a-zA-Z0-9_]*:", REG_EXTENDED);
    regcomp(&instr, "^[_a-zA-Z][a-zA-Z0-9_]*:", REG_EXTENDED);
    regcomp(&directive, "^[_a-zA-Z][a-zA-Z0-9_]*:", REG_EXTENDED);
    regcomp(&hex, "0x[a-fA-F0-9]+:", REG_EXTENDED);
    regcomp(&decimal, "[0-9]+:", REG_EXTENDED);
    
    if (match(&label, "d:"))
        printf("It matches!\n");
    else
        printf("It doens\'t match...\n");

    /* Free! */
    regfree(&label);
    regfree(&instr);
    regfree(&directive);
    regfree(&hex);
    regfree(&decimal);

    File test;

    initializeFile(&test, "test.txt");

    while(readFile(&test)) {
        printf("%s", test.buffer);
    }

    printf("\nTotal of n. of lines is: %d\n", test.line);

    cleanFile(&test);

    return 0;
}
