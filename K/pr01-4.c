#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// struct Node {
//     struct Node *prev, *next;
//     char *elem;
// };

// struct List {
//     struct Node *first, *last;
// };

void process(struct List *pl, const char *str) {
    struct Node *ptr = pl->first;
    struct Node *lastPtr = pl->last;
    struct Node *save_ptr;

    while (ptr != lastPtr) {
        save_ptr = ptr->next;

        if (strcmp(ptr->elem, str) == 0) {
            (ptr->prev)->next = ptr->next;
            (ptr->next)->prev = ptr->prev;
            free(ptr->elem);
            free(ptr);
        } else if (strcmp(ptr->elem, str) > 0) {
            (ptr->prev)->next = ptr->next;
            (ptr->next)->prev = ptr->prev;

            pl->last->next = ptr;
            ptr->prev = pl->last;
            ptr->next = NULL;
            pl->last = ptr;
        }

        ptr = save_ptr;
    }
}