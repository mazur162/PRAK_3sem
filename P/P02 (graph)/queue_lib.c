#include <stdio.h>
#include <stdlib.h>

// Узел очереди
typedef struct node {
    const void*  ptr;
    struct node* next;
} node_t;

// Очередь для обхода дерева в ширину
// Очередь работает по принципу FIFO: First In First Out
typedef struct {
    node_t* head;
    node_t* tail;
} queue_t;

void  
queue_init(queue_t* q) { 
    q->head = NULL;
    q->tail = NULL;
}

int   
queue_empty_check(queue_t* q) {
    return (q->head == NULL);
}

const void* 
queue_head(queue_t* q) {
    return (q->head)->ptr;
}

void 
queue_push(queue_t* q, const void* ptr) {
    node_t* p = (node_t*)malloc(sizeof(node_t));
    if (p != NULL) {
        p->ptr  = ptr;
        p->next = NULL;
        if (q->head == NULL) {
            q->head = p;
            q->tail = p;
        } else {
            (q->tail)->next = p;
            q->tail = p;
        }
    }
}
 
void queue_pop(queue_t* q) {
    node_t* t;
    if (q->head != NULL) {
        t = q->head;
        q->head = (q->head)->next;
        free(t);
        if (q->head == NULL) {
            q->tail = NULL;
        }
    }
}