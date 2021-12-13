#include <stdio.h>
#include <stdlib.h>
#include "queue_lib.h"

typedef struct tree_t {   
    int value;   
    int depth;  
    struct tree_t* left;       
    struct tree_t* right;
} tree_t;

tree_t *init_tree(void) {
    tree_t *node = NULL;
    return node;
}

tree_t *create_node(int value, int depth) {
    tree_t* result = malloc(sizeof(tree_t));
    if (result != NULL) {
        result->left = NULL;
        result->right = NULL;
        result->value = value;
        result->depth = depth;
    }
    return result;
}

//Для простоты восприятия кода я использую рекурсивное удаление, однако
// алгоритм нерекурсивного удаления дерева уже был мною реализован 
// в одном из домашних заданий
void 
delete_tree(tree_t *node) {
    if (node == NULL) {
        return;
    }
    delete_tree(node->left);
    delete_tree(node->right);
    free(node);
}

void 
insert_node(tree_t **node_ptr, int value, int depth) {
    tree_t *node = *node_ptr;

    if (node == NULL) {
        // tree_t empty
        (*node_ptr) = create_node(value, depth);
        return;
    }

    if (value < node->value) {
        insert_node(&(node->left), value, depth + 1);
        return;
    } else {
        insert_node(&(node->right), value, depth + 1);
        return;
    }
}


// Я считаю, что нумерация уровней глубины дерева начинается с 0
// Где 0 - глубина вершины, а максимальную глубину имеют листья дерева
int 
sum_of_elements_on_levels(tree_t* node, char parity) {
    queue_t q;
    queue_init(&q);
    int sum = 0;
    int dep = node->depth; // dep = 0
    queue_push(&q, node);

    while(!queue_empty_check(&q)) {
        tree_t* p = (tree_t*)queue_head(&q);
        queue_pop(&q);
        dep = p->depth;
        if (dep % 2 == parity) {
            sum += p->value;
        }

        if (p->left != NULL) {
            queue_push(&q, p->left);
        }
        if (p->right != NULL) {
            queue_push(&q, p->right);
        }
    }
    return sum;
}

// Бинарное дерево устроено так, что максимальный элемент находится
// в самом правом узле
int
find_max(tree_t* node) {
    tree_t* p = node;
    while (p->right != NULL) {
        p = p->right;
    } 
    return (p->value);
}

// Бинарное дерево устроено так, что минимальный элемент находится
// в самом левом узле
int
find_min(tree_t* node) {
    tree_t* p = node;
    while (p->left != NULL) {
        p = p->left;
    } 
    return (p->value);
}

char
search_value(tree_t* node, int val) {
    if (node == NULL) {
        return 0;
    }
    if (node->value == val) {
        return 1;
    }
    char result = search_value(node->left, val);
    if (result) {
        return 1;
    }

    result = search_value(node->right, val);
    if (result) {
        return 1;
    }

    return 0;
}