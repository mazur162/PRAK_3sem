#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <limits.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

struct ListItem {
    char *str;
    struct ListItem *next;
};

struct ListItem *list_dup(const struct ListItem *list) {
    struct ListItem *begin = NULL, *tmp;
    int flag = 1;
    while (list != NULL) {
        for(int i = 0; i< strlen(list->str); i++) {
            if (list->str[i] < 'a' || list->str[i] >'z') {
                flag = 0;
                break;
            }
        }
        if (flag) {
            tmp = (struct ListItem *)malloc(sizeof(struct ListItem));
            tmp->str = (char*)malloc(strlen(list->str)+1);
            strcpy(tmp->str, list->str);
            tmp->next = begin;
            begin = tmp;
        }
        flag = 1;
        list=list->next;
    }
    return begin;
}
struct ListItem * inp()
{
    struct ListItem *tmp, *begin=NULL;
    char buf[80];
    int N;
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
    {
        tmp = (struct ListItem *)malloc(sizeof(struct ListItem));
        scanf("%s", buf);
        tmp->str = (char*)malloc(strlen(buf)+1);
        strcpy(tmp->str, buf);
        tmp->next = begin;
        begin = tmp;
    }
    return begin;
}

void print_list( struct ListItem *l)
{
    while(l)
    {
        printf("%s ", l->str);
        l=l->next;
    }
    printf("\n");
}
int main(int argc, char** argv)
{
    
    struct ListItem *list = inp();
    print_list(list);
    struct ListItem *new = list_dup(list);
    print_list(new);
    return 0;
}

