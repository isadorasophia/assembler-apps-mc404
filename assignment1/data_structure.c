/* ********************************************************************* *
 * 
 *                     Data structure (implementation)
 * Description: responsible for the main structures used during the 
 * program.
 *
 * ********************************************************************* */
#include "data_structure.h"

#include <regex.h>
#include <stdio.h>

/** 
 * Functions below are used for binary search tree implementation
 */
/* Helper function used to search for a Label key */
int compar_label(const void *l, const void *r) {
    const Label* lm = l;
    const Label* lr = r;

    return strcmp (lm->key, lr->key);
}

/* Insert a value of type Label */
void insert_label(const char* key, const Position value, void** root) {
    Label* t = malloc(sizeof(Label));

    t->key = strdup(key);
    t->value = value;

    tsearch(t, root, compar_str);
}

/**
 * Find a given value in the binary search tree of type Label
 *
 * return:          if could find the value
 * result:          inserts the respective found value
 */
bool find_label (const char* key, void** root, Position* result) {
    Label* t = malloc(sizeof(Label));
    t->key = strdup(key);

    void* r = tfind(t, root, compar_str);

    if (r == NULL) {
        return false;
    } else {
        (*result) = (*(Label**)r)->value;

        return true;
    }
}

/* Helper function used to search for a String_map key */
int compar_str(const void *l, const void *r) {
    const String_map* lm = l;
    const String_map* lr = r;

    return strcmp (lm->key, lr->key);
}

/* Insert a value of type String_map */
void insert_str(const char* key, const int value, void** root) {
    String_map* t = malloc(sizeof(String_map));

    t->key = strdup(key);
    t->value = value;

    tsearch(t, root, compar_str);
}

/**
 * Find a given value in the binary search tree of type String_map
 *
 * return:          if could find the value
 * result:          inserts the respective found value
 */
bool find_str(const char* key, void** root, int* result) {
    String_map* t = malloc(sizeof(String_map));
    t->key = strdup(key);

    void* r = tfind(t, root, compar_str);

    if (r == NULL) {
        return false;
    } else {
        (*result) = (*(String_map**)r)->value;

        return true;
    }
}

/**
 * Initialize memory map
 */
void initialize_mem(MemMap* memMap) {
    for (int i; i < MAX_WORDS; i++) {
        memMap[i].used = false;

        strcpy (memMap[i].content[left], "00000");
        strcpy (memMap[i].content[right], "00000");
    }
}

/**
 * Fill dictionary of instructions
 */
void initialize_instr(void** root) {
    insert_str("LD", _LD, root);
    insert_str("LD-", _LD_MINUS, root);
    insert_str("LD|", _LD_MODULUS, root);
    insert_str("LDmq", _LDmq, root);
    insert_str("LDmq_mx", _LDmq_mx, root);
    insert_str("ST", _ST, root);
    insert_str("JMP", _JMP, root);
    insert_str("JUMP+", _JUMP_PLUS, root);
    insert_str("ADD", _ADD, root);
    insert_str("ADD|", _ADD_MODULUS, root);
    insert_str("SUB", _SUB, root);
    insert_str("SUB|", _SUB_MODULUS, root);
    insert_str("MUL", _MUL, root);
    insert_str("DIF", _DIF, root);
    insert_str("LSH", _LSH, root);
    insert_str("RSH", _RSH, root);
    insert_str("STaddr", _STaddr, root);
}

/** 
 * Helper function in order to make regex validation easier
 *
 * return:          if the given string matches the regular expression
 */
bool match(regex_t* re, const char* word) {
    // function param
    regmatch_t t[1];

    // finally, check if it matches...
    if (regexec(re, word, 1, t, 0) == 0) {
        return true;
    } else {
        return false;
    }
}

/**
 * If a given string is enclosed in quotation marks, take them off.
 * ...Or just take off whatever is enclosing the string
 *
 * check:           if wants to check before remove, or remove anyway
 * return:          if there were quotation marks
 */
bool clean_constraints(char* w, bool check) {
    int size = strlen(w);

    if (!check || w[0] == '"' && w[size - 1] == '"') {
        memmove (w, w + 1, size - 2);

        w[size - 2] = '\0';

        return true;
    } else {
        return false;
    }
}