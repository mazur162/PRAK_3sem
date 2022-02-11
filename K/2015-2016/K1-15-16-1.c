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


struct Node
{
    char *elem;
    struct Node *next;
};


struct Node *delete_all( struct Node *list, const char* str) {
    struct Node* pred = list, *cur = list, *res = list;
    //проходим по начальным звеньям списка до первого звена,
    //которое не надо удалять
    while((cur != NULL) && (strcmp(cur->elem, str) > 0)) {
        res = cur->next;
        free(cur->elem);
        free(cur);
        cur = res;
    }
    //поддерживаем два указателя - на предыдущий и
    //текущий элементы списка
    if (cur != NULL) {
        pred = cur;
        cur = cur->next;
    }
    while (cur != NULL) {
        if (strcmp(cur->elem, str)>0) {
            //удаляем звено и освобождаем память
            pred->next = cur->next;
            free(cur->elem);
            free(cur);
            cur = pred->next;
        } else {
            //просто переходим к следующему звену
            pred = pred->next;
            cur = cur->next;
        }
    }
    return res;
}
struct Node * inp()
{
    struct Node *tmp, *begin=NULL;
    char buf[80];
    int N;
    scanf("%d", &N);
    for(int i = 0; i < N; i++)
    {
        tmp = (struct Node *)malloc(sizeof(struct Node));
        scanf("%s", buf);
        tmp->elem = (char*)malloc(strlen(buf)+1);
        strcpy(tmp->elem, buf);
        tmp->next = begin;
        begin = tmp;
    }
    return begin;
}

void print_list( struct Node *l)
{
    while(l)
    {
        printf("%s ", l->elem);
        l=l->next;
    }
    printf("\n");
}

int main(int argc, char** argv)
{
    
    struct Node *list = inp();
    print_list(list);
    struct Node *new = delete_all(list, "df");
    print_list(new);
    return 0;
}
 