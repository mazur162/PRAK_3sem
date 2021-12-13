//узел очереди
typedef struct node {
    const void*  ptr;
    struct node* next;
} node_t;

//очередь(FIFO) для обхода дерева в ширину
typedef struct {
    node_t* head;
    node_t* tail;
} queue_t;


void queue_init(queue_t* q);
int queue_push(queue_t* q, const void* ptr);
void queue_pop(queue_t* q);
const void* queue_head(queue_t* q);
int queue_empty_check(queue_t* q);