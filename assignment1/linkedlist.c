/* ***********************************************************************
 * 
 *                       Linked list (implementation)
 * Description: standard linked list operations.
 *
 * ********************************************************************* */
#include "data_structure.h"
#include "linkedlist.h"

#include <string.h>

/**
 * Pop a node of the list.
 * 
 * return:                  label of removed node
 */
char* pop (Node** node) {
    // list is empty, ignore
    if (node == NULL) {
        return NULL;
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
 * Check if a given list is empty
 */
bool empty (Node* node) {
    if (node == NULL)
        return true;
    else
        return false;
}

/**
 * Clean up the whole list
 */
void clean_list (Node** node) {
    while ((*node) != NULL) {
        Node* tmp = *node;
        (*node) = (*node)->next;

        free(*node);
    }
}