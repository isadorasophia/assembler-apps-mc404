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