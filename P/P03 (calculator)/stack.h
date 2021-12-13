#include <stdio.h>

typedef struct node {
    char value;
    struct node *next;
}   stack_type;

typedef struct intnode {
    int value;
    struct intnode *next;
}   elem_t;

stack_type *init(void);

void push(stack_type **head, char value);

char pop(stack_type **head);

void clear(stack_type *head);


elem_t *int_init(void);

void int_push(elem_t **head, int value);

int int_pop(elem_t **head);

void int_clear(elem_t *head);

