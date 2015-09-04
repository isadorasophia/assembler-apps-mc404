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
 * content:         instruction to be executed
 * used:            if the word had been used
 */
typedef struct MemMap {
    char        content[2][7];
    bool        used;
} MemMap;

/* Initialize memory map */
void initializeMem(MemMap* memMap);

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

/* Search for a key */
int comparLabel(const void *l, const void *r);

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

/* Search for a key */
int comparStr(const void *l, const void *r);