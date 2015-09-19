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

/* 
 * Insert a value of type Label 
 *
 * return:         if succeeded (there are no duplicates)
 */
bool insert_label(const char* key, const Position value, void** root) {
    Label* t = malloc(sizeof(Label));

    t->key = strdup(key);
    t->value = value;

    // check if an element with given key already exists
    if (tfind(t, root, compar_label)) {
        return false;
    } else {
        tsearch(t, root, compar_label);

        return true;
    }
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

    void* r = tfind(t, root, compar_label);

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

/* 
 * Insert a value of type String_map 
 *
 * return:         if succeeded (there are no duplicates)
 */
bool insert_str(const char* key, const int value, void** root) {
    String_map* t = malloc(sizeof(String_map));

    t->key = strdup(key);
    t->value = value;

    // check if an element with given key already exists
    if (tfind(t, root, compar_str)) {
        return false;
    } else {
        tsearch(t, root, compar_str);

        return true;
    }
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

/**
 * Updates the position to goal position or to the next 
 * one available, report an error if it was not possible.
 *
 * to:              goal position or GO_NEXT to go to next
 */
void update_position(Position* p, const int to) {
    if (to == GO_NEXT) {
        if ((*p).state == left) {
            (*p).state = right;
        } else {
            if ((*p).address + 1 < MAX_WORDS) {
                (*p).state = left;
                (*p).address++;
            }

            // if the no. of words has exceeded
            else {
                report_error(file.out, "Program has exceeded the limit of words!", -1, 1);
            }
        }
    } 

    // go to arbitrary position
    else {
        if (to + 1 < MAX_WORDS) {
            (*p).state = left;
            (*p).address = to;
        }

        // if the no. of words has exceeded
        else {
            report_error(file.out, "Program has exceeded the limit of 
                words!", -1, 1);
        }
    }
}

/**
 * Set all the labels from a list to a corresponding memory
 */
void set_labels(Node** l, String_map** labels, int mem, int line) {
    char* tmp;

    while (l != NULL) {
        tmp = pop(l);

        // check if something went wrong
        if (!insert_str((*l)->label, mem, labels)) {
            report_error(file.out, strcat((*l)->label, 
                    " was already declared!"), -1, 1);
        }

        free(tmp);
    }
}

/** 
 * Read an argument converted in string, either in hex or decimal.
 * Report an error if it wasn't declared correctly.
 *
 * return:          its value in decimal 
 */
int read_constant(char* buffer, int line) {
    int tmp_int;

    // is it decimal?
    if (match(&dec_regex, buffer)) {
        // save value
        tmp_int = atoi(buffer);
    }

    // is it hex?
    else if (match(&hex_regex, buffer)) {
        // save value as decimal
        sscanf(buffer, "%x", &tmp_int);
    }

    // none of them, report an error!
    else {
        report_error(file.out, strcat(buffer, " is not a valid argument!"), line, 1);
    }

    return tmp_int;
}

/**
 * Find minimum multiple of n above value of min
 *
 * return:          result
 */
int min_mul(int n, int mim) {
    int division = mim / n;

    // if the division wasnt an integer, pick up the min above
    if (division != 0) {
        division += n;
    }

    return division;
}