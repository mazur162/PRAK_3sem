#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct list_item {
    char *str;
    struct list_item *next, *prev;
};

void
process(struct list_item **list, const char *s) {
    struct list_item *ptr = *list;
    struct list_item *last = ptr->prev;
    do {
        struct list_item *nextptr = ptr->next;
        if (strstr(ptr->str, s)) {
            for (int i = 0; i < strlen(ptr->str); i++) {
                struct list_item *new = malloc(sizeof(struct list_item));
                new->str = malloc(strlen(ptr->str) + 1);
                strcpy(new->str, ptr->str);
                
                new->next = ptr->next;
                new->prev = ptr;
                ptr->next = new;
                new->next->prev = new;
            }
            if (ptr == last) {
                last = nextptr->prev;
            }
        } else {
            if (ptr == (*list)) {
                (*list) = (*list)->next;
            } else {
                if (ptr == last) {
                    last = last->prev;
                }
                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;
                ptr->next = (*list);
                ptr->prev = (*list)->prev;
                (*list)->prev->next = ptr;
                (*list)->prev = ptr;
            }
        }
        ptr = nextptr;

    } while (ptr != last->next);
}

int
main(void) {
    return 0;
}