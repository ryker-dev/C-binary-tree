#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int value;
    short balance;
    struct node *left, *right;
} node, *tree_pointer;

void add_node(tree_pointer *, int);
void print_tree(tree_pointer);

int main(int argc, char const *argv[])
{
    int etp, i = 0, input[] = {2, 4, 6, 8, 10, 12, 14, 28, 30};
    tree_pointer root = NULL;

    for(i = 0; input[i] != 0; i++) {
        add_node(&root, input[i]);
    }

    print_tree(root);

    return 0;
}

void add_node(tree_pointer *parent, int value) {
    // If there is no root (first element)
    if(!(*parent)) {
        //*etp = 1;
        if(!(*parent = (tree_pointer)malloc(sizeof(node)))) {
            perror("Malloc: Failed to reserve memory for a node!");
            exit(1);
        }
        (*parent)->left = (*parent)->right = NULL;
        (*parent)->balance = 0;
        (*parent)->value = value;
    }

    else if(value < (*parent)->value) {
        add_node(&(*parent)->left, value);
    }

    else if(value > (*parent)->value) {
        add_node(&(*parent)->right, value);
    }
}

void print_tree(tree_pointer node)
{
    if(!node) return;
    print_tree(node->left);
    printf("%d ", node->value);
    print_tree(node->right);
}