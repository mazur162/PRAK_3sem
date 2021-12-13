#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

stack_type *init(void) {
    stack_type *tmp = NULL;
    return tmp;
}

void push(stack_type **head, char value) {
    stack_type *tmp = malloc(sizeof(stack_type));
    if(tmp == NULL) {
        exit(-1);
    }
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}

char pop(stack_type **head) {
    stack_type *out;
    char value;
    if (*head == NULL) {
        exit(-2);
    }
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}

void clear(stack_type *head) {
    if (head == NULL) return;
    while (head) 
        pop(&head);
}


elem_t *int_init(void) {
    elem_t *tmp = NULL;
    return tmp;
}

void int_push(elem_t **head, int value) {
    elem_t *tmp = malloc(sizeof(elem_t));
    if(tmp == NULL) {
        exit(-1);
    }
    tmp->next = *head;
    tmp->value = value;
    *head = tmp;
}

int int_pop(elem_t **head) {
    elem_t *out;
    int value;
    if (*head == NULL) {
        exit(-2);
    }
    out = *head;
    *head = (*head)->next;
    value = out->value;
    free(out);
    return value;
}

void int_clear(elem_t *head) {
    if (head == NULL) return;
    while (head) 
        int_pop(&head);
}
