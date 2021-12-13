#include <stdio.h>
#include <stdlib.h>
#include "bin_tree_lib.h"
#include "bin_tree_input_output.h"

int
main(int argc, char** argv) {
    tree_t *node = input_output(argc, argv);
    delete_tree(node);
    return 0;
}