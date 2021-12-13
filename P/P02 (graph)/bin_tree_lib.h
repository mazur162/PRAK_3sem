typedef struct tree_t {   
    int value;    
    int depth;  
    struct tree_t* left;       
    struct tree_t* right;
} tree_t;

tree_t *init_tree(void);
tree_t *create_node(int value, int depth);
void delete_tree(tree_t *node);
void insert_node(tree_t **node_ptr, int value, int depth);
int sum_of_elements_on_levels(tree_t* node, char parity);
int find_max(tree_t* node);
int find_min(tree_t* node);
char search_value(tree_t* node, int val);