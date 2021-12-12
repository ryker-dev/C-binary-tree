#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    short height;
    struct node *left, *right;
} node, *tree_pointer;

int get_height(node *node)
{
    if (node == NULL)
        return 0;
    return node->height;
}


int calc_balance(node *node)
{
    if (node == NULL)
        return 0;
    return get_height(node->left) - get_height(node->right);
}

int max(int a, int b)
{
    return (a > b)? a : b;
}

/* struct node* create_node(int value)
{
    if(!(node* new_node = (tree_pointer)malloc(sizeof(node)))) {
        perror("Malloc: Failed to reserve memory for a node!");
        exit(1);
    }
    new_node->left = new_node->right = NULL;
    new_node->height = 1;
    new_node->value = value;
    return(new_node);
} */

void add_node(tree_pointer *parent, int value) {
    // If there is no root (first element)
    if(!(*parent)) {
        //*etp = 1;
        if(!(*parent = (tree_pointer)malloc(sizeof(node)))) {
            perror("Malloc: Failed to reserve memory for a node!");
            exit(1);
        }
        (*parent)->left = (*parent)->right = NULL;
        (*parent)->height = 1;
        (*parent)->value = value;
    }

    else if(value < (*parent)->value) {
        add_node(&(*parent)->left, value);
    }

    else if(value > (*parent)->value) {
        add_node(&(*parent)->right, value);
    }
}

/* void left_rotate(tree_pointer) {

} */

void print_tree(tree_pointer node)
{
    if(!node) return;
    print_tree(node->left);
    printf("%d ", node->value);
    print_tree(node->right);
}

int main(int argc, char const *argv[])
{
    int etp, i = 0, input[] = { 2, 4, 6, 8, 10, 12, 14, 28, 30};
    tree_pointer root = NULL;

    for(i = 0; input[i] != 0; i++) {
        add_node(&root, input[i]);
    }

    print_tree(root);

    return 0;
}