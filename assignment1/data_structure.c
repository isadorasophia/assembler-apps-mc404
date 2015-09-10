#include "data_structure.h"

#include <string.h>

/** 
 * Functions below are used for binary search tree implementation
 */
int comparLabel(const void *l, const void *r) {
    const Label* lm = l;
    const Label* lr = r;

    return strcmp (lm->key, lr->key);
}

int comparStr(const void *l, const void *r) {
    const String_map* lm = l;
    const String_map* lr = r;

    return strcmp (lm->key, lr->key);
}

/**
 * Initialize memory map
 */
void initializeMemory(MemMap* memMap) {
    for (int i; i < MAX_WORDS; i++) {
        memMap[i].used = false;

        strcpy (memMap[i].content[left], "00000");
        strcpy (memMap[i].content[right], "00000");
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