/* ***********************************************************************
 * 
 *                              Linked list
 * Description: standard linked list operations.
 *
 * ********************************************************************* */
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

/**
 * Node structure of linked list
 */
typedef struct Node {
    char*           label;
    struct Node*    next;
} Node;

/* Default functions */
char* pop (Node**);
void push (Node**, char*);
int empty (Node*);
void clean_list (Node**);

#endif