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

#include <regex.h>                  // regular expressions
#include <search.h>                 // enables binary tree search

#include "data_structure.h"
#include "linkedlist.h"

int main () {
    printf("Testing binary tree...\n");

    void* root = NULL;

    String_map* a = malloc (sizeof(String_map));
    a->key = strdup("aaaa");
    a->value = 1;

    tsearch(a, &root, comparStr);

    String_map* find_a = malloc(sizeof(String_map));
    find_a->key = strdup("aaaa");

    void* r = tfind(find_a, &root, comparStr); /* read */
    printf("Result is %d, success!\n", (*(String_map**)r)->value);

    printf("Testing linked list...\n");
    Node* d = NULL;
    char* b;

    push(&d, "hi");

    b = strdup(pop(&d));

    if (empty(d)) {
        printf("Result is %s, success!\n", b);
    }

    free(b);

    return 0;
}
