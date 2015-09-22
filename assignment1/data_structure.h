/* ********************************************************************* *
 * 
 *                             Data structure
 * Description: responsible for the main structures used during the 
 * program.
 *
 * ********************************************************************* */
#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include <regex.h>              // regex validation and definition
#include <stdio.h>

#include "linkedlist.h"

/* Regex definition */
#define LABEL_REGEX "^[_a-zA-Z][a-zA-Z0-9_]{0,63}:$"    // .label:
#define SYM_REGEX "^[_a-zA-Z][a-zA-Z0-9_]{0,63}$"       // SYM
#define INSTR_REGEX "^[A-Z][A-Za-z]+"                   // INSTRUCTION
#define DIRECTIVE_REGEX "^\\.[a-z]+$"                    // .directive
#define HEX_REGEX "^0x[a-fA-F0-9]{10}$"                 // 0x000...
#define DEC_REGEX "^-?[0-9]{0,4}$"                      // 1

/* Instructions definition */
#define _LD 1
#define _LD_MINUS 2
#define _LD_MODULUS 3
#define _LDmq 10
#define _LDmq_mx 9
#define _ST 33
#define _JMP 13
#define _JUMP_PLUS 15
#define _ADD 5
#define _ADD_MODULUS 7
#define _SUB 6
#define _SUB_MODULUS 8
#define _MUL 11
#define _DIF 12
#define _LSH 20
#define _RSH 21
#define _STaddr 18

/* Max size of the memory map */
#define MAX_WORDS 1024

/**
 * Attempt to create boolean values
 */
typedef int bool;
#define true    1
#define false   0

/**
 * Left and right values
 */
#define left    0
#define right   1

/**
 * Flag used to handle the position function
 */
#define GO_NEXT -1

/**
 * Word size in hex
 */
#define WORD_SIZE 10

/**
 * Handler
 */
#define SKIP -1

#define ld long int

/* Default model of each text line */
typedef enum Model { LABEL, INSTR_OR_DIR, END } Model;

/**
 * Data structure to inform a position in the final output.
 *
 * address:         the current word in the map address
 * state:           left or right of the word
 */
typedef struct Position {
    int         address;
    int         state;
} Position;

/**
 * Memory map
 *
 * content:         instruction to be executed (left/right)
 * used:            if the word had been used
 */
typedef struct MemMap {
    char        content[2][7];
    bool        used;
} MemMap;

/* Initialize memory map */
void initialize_mem(MemMap*);

/**
 * Data estructure to simulates a std::map regarding label
 *
 * key:             name of the label
 * value:           position of the label
 */
typedef struct Label {
    char*       key;
    Position    value;
} Label;

/* Helper function used to search for a Label key */
int compar_label(const void*, const void*);

/* Insert a value */
bool insert_label(const char*, const Position, void**, bool);

/* Find a given value in the binary search tree of type Label */
bool find_label (const char*, void**, Position*, bool);

/**
 * General data estructure to simulates a std::map<String>
 *
 * key:             string
 * value:           instruction translated into machine language
 */
typedef struct String_map {
    char*       key;
    ld          value;
} String_map;

/* Helper function used to search for a String_map key */
int compar_str(const void*, const void*);

/* Insert a value of type String_map */
bool insert_str(const char*, ld, void**, bool);

/* Find a given value in the binary search tree of type String_map */
bool find_str (const char*, void**, ld*, bool);


/* Fill dictionary of instructions */
void initialize_instr(void**);

/* Set all the labels from a list to a corresponding memory */
void set_labels(Node**, void**, Position, int, FILE*);

/* In order to make regex validation easier */
bool match(regex_t*, const char*);
bool clean_constraints(char*, bool);

/* Updates the position to goal or simply to the next one */
void update_position(Position*, const int, FILE*);

/**
 * Helper task functions
 */
/* Find minimum multiple of n above value of min */
int min_mul(int, int);

/* Check if current position is placed in a
 * a full 40 bit word */
void check_40bit(Position, const char*, int, FILE*);

/**
 * Memory map functions
 */
/* Turn a ld number into a hex string */
void hex_string(ld, char*, int);

/* Fill a word with a constant value in current position
 * and proceed to next available memory */
void fill_word(MemMap*, Position*, char*, FILE*);

/* Copy word content into buffer */
void copy_word(MemMap*, int, char*);

/* Custom implementation of strlwr, 
 * which turns a string into lowercase only */
void strlwr(char* str);

#endif