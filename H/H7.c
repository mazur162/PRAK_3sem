/*
- I know, that my output is a little bit ugly :)
- To delete tree without recursion I use following algorithm: 
    moving everything to one side, to create only one sub-tree
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct tree {   
    char c; 
    int n;            
    struct tree* left;       
    struct tree* right;
} tree;

tree *create_node(char c, int n) {
    tree* result = malloc(sizeof(tree));
    if (result != NULL) {
        result->left = NULL;
        result->right = NULL;
        result->c = c;
        result->n = n;
    }
    return result;
}

void delete_tree(tree *node) {
    int delete_counter = 0;

    while (node != NULL) {
        tree *temp_pointer = node;
        if (node->left != NULL) {
            node = node->left;
            temp_pointer->left = node->right;
            node->right = temp_pointer;
        } else {
            node = node->right;
            free(temp_pointer);
            delete_counter++;
        }
    }
}

// void delete_tree_rec(tree *node) {
//     if (node == NULL) {
//         return;
//     }
//     delete_tree(node->left);
//     delete_tree(node->right);
//     free(node);
// }

void print_tree(tree *node, char side) { // 0 - left, 1 - right
    static int k = 4;
    k+=2;
    if (side == 0) {
        k-=2;
    } else {
        k+=2;
    }  for (int i = 0; i < k; i++) {
        printf("  ");
    }
    k--;

    if (node == NULL) {
        //printf("empty");
        return;
    }
    printf("%c(%d)\n", node->c, node->n);
    //printf("\n");

    print_tree(node->left, 0);
    printf("  ");
    print_tree(node->right, 1);
    printf("\n");
    return;

}


void insert_node(tree **node_ptr, char c) {
    tree *node = *node_ptr;

    if (node == NULL) {
        // tree empty
        int n = 1;
        (*node_ptr) = create_node(c, n);
        return;
    }

    if (c == node->c) {
        node->n++;
        return;
    }

    if (c < node->c) {
        insert_node(&(node->left), c);
        return;
    } else {
        insert_node(&(node->right), c);
        return;
    }
}

int main(void) {
    int MAX_LEN = 10000;
    char string[MAX_LEN + 2];
    char *s;

    s = fgets(string, MAX_LEN + 2, stdin);
    s[strlen(s) - 1] = '\0'; // deleting '\n'

    tree *node = NULL;

    while (*s != '\0') {
        insert_node(&node, *s);
        s++;
    }

    print_tree(node, 1);
    delete_tree(node);
    return 0;
}