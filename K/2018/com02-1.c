#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
struct ListItem {
	unsigned char *str;
	struct ListItem *prev;
	struct ListItem *next;
};
 
void process(struct ListItem *head, unsigned char *prefix) {
	struct ListItem *ptr = head;
	while (ptr != NULL) {
		unsigned char *podstr;
		if ((podstr = strstr(ptr->str, prefix)) != NULL) {
			if (podstr == ptr->str) {
				podstr += strlen(prefix);
				char *strend;
				int num = strtol(podstr, &strend, 10);
				
				// printf("%s %d\n", num, *podstr);
				if (*strend == '\0') {
					int count = 0;
					while (count < num && ptr->next != NULL) {
						struct ListItem *del = ptr->next;
						ptr->next = del->next;
						if (del->next != NULL) {
							del->next->prev = ptr;
						}
						free(del->str);
						free(del);
						count++;
					}
				}
			}
		}
		ptr = ptr->next;
	}
}


struct ListItem *
read_list(void)
{
	struct ListItem *begin = NULL;
	char name[100];
	int c, i = 0, flag = 1;
	do {
		c = getchar();
		if (isspace(c)) {
			name[i] = '\0';
			struct ListItem *ls = malloc(sizeof(struct ListItem));
			ls->str = malloc(strlen(name) + 1);
			strcpy(ls->str, name);
//			printf("%s\n", name);
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
	begin->prev->next = NULL;
	begin->prev = NULL;
	return begin;
}

void
print_list(struct ListItem *ls)
{
	struct ListItem *curr = ls;
	while (curr != NULL) {
		printf("%s ", curr->str);
		curr = curr->next;
	}
	printf("\n");
}

void
free_list(struct ListItem *ls)
{
	struct ListItem *curr = ls;
	struct ListItem *nextptr;
	while (curr != NULL) {
		nextptr = curr->next;
		free(curr->str);
		free(curr);
		curr = nextptr;
	}
	printf("\n");
}
 
 int main(void) {
	 struct ListItem *ls = read_list();
	 print_list(ls);
	 process(ls, "abc");
	 print_list(ls);
	 free_list(ls);
	 
}
