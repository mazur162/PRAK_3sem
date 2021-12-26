#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

typedef struct Elem {
    struct Elem *next, *prev;
    char *str;
} Elem;

struct Elem *
read_list(void)
{
	struct Elem *begin = NULL;
	char name[100];
	int c, i = 0, flag = 1;
	do {
		c = getchar();
		if (isspace(c)) {
			name[i] = '\0';
			struct Elem *ls = malloc(sizeof(struct Elem));
			ls->str = malloc(strlen(name) + 1);
			strcpy(ls->str, name);
			if (flag) {
				begin = ls;
				ls->next = ls;
				ls->prev = ls;
				flag = 0;
			} else {
				ls->prev = begin->prev;
				ls->next = begin;
				begin->prev->next = ls;
				begin->prev = ls;
			}
			i = 0;
		} else {
			name[i++] = (char) c;
		}
	} while (c != EOF && c != '\n');
	return begin;
}

void
print(Elem *head) {
    Elem *ptr;
    ptr = head;
    do {
        printf("%s ", ptr->str);
        ptr = ptr->next;
    } while (ptr != head);
}

Elem *process (Elem *head) {
    Elem *ptr;
    int size = 100;
    char *string = malloc(size * sizeof(char));
    ptr = head;
    do {
        if (strcmp(string, ptr->str) == 0) {
            (ptr->prev)->next = ptr->next;
            (ptr->next)->prev = ptr->prev;
            free(ptr->str);
            free(ptr);
        } else {
            if (strlen(ptr->str) > size) {
                size = size * 2;
                string = realloc(string, size * sizeof(char));
            }
            strcpy(string, ptr->str);
        }
        ptr = ptr->next;
    } while (ptr != head);
    ptr = head;
    Elem *max = ptr;
    do {
        if (strstr(max->str, ptr->str) >= 0) {
            max = ptr;
        }
        ptr = ptr->next;
    } while (ptr != head);
    return max;
}

int
main(void) {
    struct Elem *list = read_list();
    print(list);
    printf("\n");
    printf("max: %s", (process(list))->str);
    printf("\n");
    print(list);
    printf("\n");
    return 0;
}