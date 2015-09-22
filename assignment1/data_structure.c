/* ********************************************************************* *
 * 
 *                     Data structure (implementation)
 * Description: responsible for the main structures used during the 
 * program.
 *
 * ********************************************************************* */
#include "data_structure.h"

#include <string.h>

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
 * sensitive:      if it should be case sensitive
 * return:         if succeeded (there are no duplicates)
 */
bool insert_label(const char* key, const Position value, void** root, 
                  bool sensitive) {
    Label* t = malloc(sizeof(Label));

    // copy string for further handling
    int s = strlen(key);
    char key_aux[s + 1];

    strcpy(key_aux, key);

    if (!sensitive) {
        // convert into lowercase
        strlwr(key_aux);
    }

    t->key = strdup(key_aux);
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
 * sensitive:       if it should be case sensitive
 * result:          inserts the respective found value
 * return:          if could find the value
 */
bool find_label(const char* key, void** root, Position* result,
                bool sensitive) {
    Label* t = malloc(sizeof(Label));

    // copy string for further handling
    int s = strlen(key);
    char key_aux[s];
    strcpy(key_aux, key);

    if (!sensitive) {
        // convert into lowercase
        strlwr(key_aux);
    }

    t->key = strdup(key_aux);

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
 * sensitive:       if it should be case sensitive
 * return:          if succeeded (there are no duplicates)
 */
bool insert_str(const char* key, ld value, void** root, bool sensitive) {
    String_map* t = malloc(sizeof(String_map));

    // copy string for further handling
    int s = strlen(key);
    char key_aux[s];
    strcpy(key_aux, key);

    if (!sensitive) {
        // convert into lowercase
        strlwr(key_aux);
    }

    t->key = strdup(key_aux);
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
 * sensitive:       if it should be case sensitive
 * result:          inserts the respective found value
 * return:          if could find the value
 */
bool find_str(const char* key, void** root, ld* result, bool sensitive) {
    String_map* t = malloc(sizeof(String_map));

    // copy string for further handling
    int s = strlen(key);
    char key_aux[s];
    strcpy(key_aux, key);

    if (!sensitive) {
        // convert into lowercase
        strlwr(key_aux);
    }

    t->key = strdup(key_aux);

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
    for (int i = 0; i < MAX_WORDS; i++) {
        memMap[i].used = false;

        strcpy(memMap[i].content[left], "00 000");
        strcpy(memMap[i].content[right], "00 000");
    }
}

/**
 * Fill dictionary of instructions
 */
void initialize_instr(void** root) {
    insert_str("LD", _LD, root, true);
    insert_str("LD-", _LD_MINUS, root, true);
    insert_str("LD|", _LD_MODULUS, root, true);
    insert_str("LDmq", _LDmq, root, true);
    insert_str("LDmq_mx", _LDmq_mx, root, true);
    insert_str("ST", _ST, root, true);
    insert_str("JMP", _JMP, root, true);
    insert_str("JUMP+", _JUMP_PLUS, root, true);
    insert_str("ADD", _ADD, root, true);
    insert_str("ADD|", _ADD_MODULUS, root, true);
    insert_str("SUB", _SUB, root, true);
    insert_str("SUB|", _SUB_MODULUS, root, true);
    insert_str("MUL", _MUL, root, true);
    insert_str("DIF", _DIF, root, true);
    insert_str("LSH", _LSH, root, true);
    insert_str("RSH", _RSH, root, true);
    insert_str("STaddr", _STaddr, root, true);
}

/**
 * Set all the labels from a list to a corresponding memory
 */
void set_labels(Node** l, void** labels, Position mem, int line, 
                FILE* output) {
    char* tmp;

    while (*l != NULL) {
        tmp = pop(l);

        // check if something went wrong
        if (!insert_label(tmp, mem, labels, false)) {
            report_error(output, strcat((*l)->label, 
                    " was already declared!"), -1, 1);
        }

        free(tmp);
    }
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
    } else if (w[size - 1] == ":") {
        memmove(w, w, size - 1);

        w[size - 1] = '\0';

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
void update_position(Position* p, const int to, FILE* output) {
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
                report_error(output, "Program has exceeded the limit of words!", -1, 1);
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
            report_error(output, 
                         "Program has exceeded the limit of words!", -1, 1);
        }
    }
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

/**
 * Check if current position is place in a
 * a full 40 bit word.
 */
void check_40bit(Position p, const char* buffer, int line, FILE* output) {
    if (p.state == right) {
        report_error(output, strcat(buffer, 
                     " is declared in non-aligned word!"), 
                     line, 1);
    }
}

/**
 * Turn a ld number into a hex string
 *
 * buffer:          hex string to be returned
 * max:             max size of string
 */
void hex_string(ld number, char* buffer, int max) {
    int s = 0;

    // turn value into hex string
    sprintf(buffer, "%.10lX", number);

    s = strlen(buffer);

    // if exceeds 10 digits, get the least significant
    if (s > max) {
        memcpy(buffer, buffer + s - max, max + 1);
    }

    // finally, format string
    format_hex(buffer);
}

/**
 * Format a hex string into default output, i.e., XX XXX XX XXX
 *
 * hex:             buffer must be bigger than WORD_SIZE + SPACE
 */
void format_hex(char* hex) {
    char aux[WORD_SIZE + SPACES];

    memcpy(aux, hex, 2);
    aux[2] = ' ';

    memcpy(aux + 3, hex + 2, 3);
    aux[6] = ' ';

    memcpy(aux + 7, hex + 5, 2);
    aux[9] = ' ';

    memcpy(aux + 10, hex + 7, 3);

    // end
    aux[13] = '\0';

    strcpy(hex, aux);
}

/**
 * Fill a word with a given string in current position
 * and proceed to next available memory.
 */
void fill_word(MemMap* map, Position* cur_pos, char* hex, FILE* output) {
    // set memory as used
    map[(*cur_pos).address].used = true;

    // apply first bigger digits
    memcpy(map[(*cur_pos).address].content[(*cur_pos).state], 
           hex, 6);

    map[(*cur_pos).address].content[(*cur_pos).state][7] = '\0';

    update_position(cur_pos, GO_NEXT, output);

    // remaining digits
    memcpy(map[(*cur_pos).address].content[(*cur_pos).state], 
           hex + 7, 6);

    map[(*cur_pos).address].content[(*cur_pos).state][7] = '\0';

    update_position(cur_pos, GO_NEXT, output);
}

/**
 * Copy word content into buffer.
 */
void copy_word(MemMap* map, int target_pos, char* buffer) {
    int target_state = left;

    /* Copy first 20 bits */
    strcpy(buffer, map[target_pos].content[target_state]);

    target_state++;

    /* Copy remaining bits */
    strcpy(buffer, map[target_pos].content[target_state]);
}

/**
 * Custom implementation of strlwr, 
 * which turns a string into lowercase only.
 */
void strlwr(char *str) {
    int i, len = strlen(str);

    for (int i = 0; i < len; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }

    return str;
}