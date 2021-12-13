#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "bin_tree_lib.h"
#include "queue_lib.h"

enum {MAX = 255};

void 
print_tree(tree_t* node, int N) {
    queue_t q;
    queue_init(&q);
    int dep = node->depth; // dep = 0
    char space_str[MAX];
    for (int i = 0; i < 2*N; i++) {
        space_str[i] = ' ';
    }
    space_str[2*N] = '\0';

    queue_push(&q, node);
    while(!queue_empty_check(&q)) {
        tree_t* p = (tree_t*)queue_head(&q);
        queue_pop(&q);

        if (dep == 0) {
            printf("%s", space_str);
        }
        if ((dep == p->depth) && (dep != 0)) {
            space_str[N - dep] = '\0';
            printf("%s%d ", space_str, p->value);
        } else {
            dep = p->depth;
            if (strlen(space_str) > 1 ) {
                space_str[strlen(space_str) - 1] = '\0';
            }
            printf("\n");
            printf("%s%d ", space_str, p->value);
        }

        if (p->left != NULL) {
            queue_push(&q, p->left);
        }
        if (p->right != NULL) {
            queue_push(&q, p->right);
        }
    }
}

tree_t *
input_output(int argc, char** argv) {
    int N;

    tree_t *node = init_tree();

    if (strcmp(argv[1], "random") == 0) {
        printf("Enter number of elements in binary tree: ");
        if ((scanf("%d", &N) != 1)||(N < 1)) {
            printf("Error: Wrong input\n");
            exit(1);
        }
        printf("Enter borders for randomize:\n");
        printf("Left border: ");
        int left_border;
        if (scanf("%d", &left_border) != 1) {
            printf("Error: Wrong input\n");
            exit(1);
        }

        printf("Right border: ");
        int right_border;
        if (scanf("%d", &right_border) != 1) {
            printf("Error: Wrong input\n");
            exit(1);
        }

        printf("\nRandom generated elements: ");
        for (int i = 0; i < N; i++) {
            int x = rand() % (right_border + 1 - left_border) + left_border;
            printf("%d ", x);
            insert_node(&node, x, 0);
        }
    } else if (strcmp(argv[1], "manual") == 0) {
        printf("Enter number of elements in binary tree: ");
        if ((scanf("%d", &N) != 1)||(N < 1)) {
            printf("Error: Wrong input\n");
            exit(1);
        }

        printf("Enter %d elements: ", N);
        for (int i = 0; i < N; i++) {
            int x;
            if (scanf("%d ", &x) != 1) {
                printf("Error: Wrong input\n");
                exit(1);
            };
            insert_node(&node, x, 0);
        }
    } else if (strcmp(argv[1], "test1") == 0) {
        printf("N = 7\n");
        printf("Elements: 10 5 19 17 21 1 6\n");
        N = 7;
        insert_node(&node, 10, 0);
        insert_node(&node, 5, 0);
        insert_node(&node, 19, 0);
        insert_node(&node, 17, 0);
        insert_node(&node, 21, 0);
        insert_node(&node, 1, 0);
        insert_node(&node, 6, 0);
    } else if (strcmp(argv[1], "test2") == 0) {
        printf("N = 1\n");
        printf("Element: 208\n");
        N = 1;
        insert_node(&node, 208, 0);
    } else {
        printf("Error: wrong command line argument\n");
        exit(1);
    }

    printf("\nBinary tree:\n");
    print_tree(node, N);
    printf("\n");

    printf("\nSum of elements on odd levels: %d\n", sum_of_elements_on_levels(node, 1));
    printf("Sum of elements on not odd levels: %d\n", sum_of_elements_on_levels(node, 0));
    printf("Max: %d\n", find_max(node));
    printf("Min: %d\n", find_min(node));

    int val;
    printf("Enter value, that you want to find: ");
    if (scanf("%d", &val) != 1) {
        printf("Error: Wrong input\n");
        exit(1);
    };
    if (search_value(node, val) == 1) {
        printf("FOUND\n");
    } else {
        printf("NOT FOUND\n");
    }

    return node;
}