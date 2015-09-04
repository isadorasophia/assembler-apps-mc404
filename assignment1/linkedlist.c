#include "data_structure.h"
#include "linkedlist.h"

#include <string.h>

/**
 * Pop a node of the list, returning label of removed node
 */
char* pop (Node** node) {
    // list is empty, ignore
    if (node == NULL) {
        return;
    }
    else {
        Node* tmp = *node;

        // get the label element
        char* _label = strdup(tmp->label);

        // delete from list
        *node = (*node)->next;
        free (tmp);

        return _label;
    }
}

/**
 * Attempt to push an already validaded item to list
 */
void push (Node** node, char* item) {
    unsigned int size = strlen(item);

    Node* new_node = malloc (sizeof(Node));

    new_node->label = strdup(item);
    new_node->next = (*node);

    *node = new_node;
}

/**
 * Return if a given list is empty
 */
bool empty (Node* node) {
    if (node == NULL)
        return true;
    else
        return false;
}