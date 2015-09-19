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

/* Regex definition */
#define LABEL_REGEX "^\.[_a-zA-Z][a-zA-Z0-9_]{0,63}:$"  // .label:
#define SYM_REGEX "^[_a-zA-Z][a-zA-Z0-9_]{0,63}$"       // SYM
#define INSTR_REGEX "^[A-Z][A-Za-z]+"                        // INSTRUCTION
#define DIRECTIVE_REGEX "^\.[a-z]+$"                    // .directive
#define HEX_REGEX "^0x[a-fA-F0-9]{10}$"                 // 0x000...
#define DEC_REGEX "^[0-9]{0,4}$"                        // 1

/* Variables that apply each regex pattern */
extern regex_t      label_regex, sym_regex,
                    instr_regex, directive_regex,
                    hex_regex, decimal_regex;

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
void initialize_mem(MemMap* memMap);

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
int compar_label(const void *l, const void *r);

/* Insert a value */
void insert_label(const char* key, const Position value, void** root);

/* Find a given value in the binary search tree of type Label */
bool find_label (const char* key, void** root, Position* result);

/**
 * General data estructure to simulates a std::map<String>
 *
 * key:             string
 * value:           instruction translated into machine language
 */
typedef struct String_map {
    char*       key;
    int         value;
} String_map;

/* Helper function used to search for a String_map key */
int compar_str(const void *l, const void *r);

/* Insert a value of type String_map */
void insert_str(const char* key, const int value, void** root);

/* Find a given value in the binary search tree of type String_map */
bool find_str (const char* key, void** root, int* result);


/* Fill dictionary of instructions */
void initialize_instr(void** root);

/* In order to make regex validation easier */
bool match(regex_t* re, const char* word);
bool clean_constraints(char* w, bool check);

/* Updates the position to goal or simply to the next one */
void update_position(Position* p, const int to);

/* Set all the labels from a list to a corresponding memory */
void set_labels(Node** l, String_map** labels, int mem);

/* Read a number converted in string, either in hex or decimal */
int read_constant(char* buffer, int line);

/* Find minimum multiple of n above value of min */
int min_mul(int n, int mim)

#endif