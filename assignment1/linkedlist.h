/**
 * Node structure of linked list
 */
typedef struct Node {
    char*           label;
    struct Node*    next;
} Node;

/* Default functions */
char* pop (Node** node);
void push (Node** node, char* item);
bool empty (Node* node);