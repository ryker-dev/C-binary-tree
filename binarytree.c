/*
Balanced Binary Tree in C
BM40A0301 Tietorakenteet ja algoritmit

Author: Emilio Pizzuti

This program uses a modified version of the code given in exercise 7 as a framework.



*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 8
#define FILENAME_SIZE 32

typedef struct node_struct
{
    int value;
    short balance; // Balance indicator. 1 = rotate left, 0 = balanced, -1 = rotate right
    struct node_struct *left, *right;
} node, *t_pointer;

/* typedef struct linked_values
{
    int value;
    struct linked_values *prev, *next;
}
 */
void add_node(t_pointer *, int, int *);
void rotate_right(t_pointer *, int *);
void print_tree(t_pointer, int);
void rotate_left(t_pointer *, int *);
void read_file(const char *, int *);
int count_lines(const char *);

int main(int argc, char const *argv[])
{
    int bi = 0, i; // bi = balance indicator
    t_pointer tree = NULL;
    /* char filename[FILENAME_SIZE] = argv[1]; */

    printf("%d\n", argv[1]);

    int values[count_lines(argv[1])];
    read_file(argv[1], values);

    for (i = 0; values[i] != 0; i++)
    {
        add_node(&tree, values[i], &bi);
        /*     print_tree(tree, 0);
            printf("\n"); */
    }
    print_tree(tree, 0);
    printf("\n");

    return 0;
}

int count_lines(const char filename[])
{
    FILE *fp;
    int lines = 0;

    fp = fopen("values.txt", "r");
    if (fp == NULL)
    {
        perror("FILE: Failed to open file. Exiting with code 1.");
        exit(1);
    }

    char c;
    while (!feof(fp))
    {
        c = fgetc(fp);
        if (c == '\n')
        {
            lines++;
        }
    }

    return lines;
}

void read_file(const char filename[], int values[])
{
    filename = "values.txt";
    FILE *fp;
    char line[BUFFER_SIZE];
    int i = 0;

    printf("%d\n", filename);

    fp = fopen("values.txt", "r");
    if (fp == NULL)
    {
        perror("FILE: Failed to open file. Exiting with code 1.");
        exit(1);
    }

    while (fgets(line, BUFFER_SIZE, fp) != NULL)
    {
        values[i++] = atoi(line);
    }

    return;
}

/* linked_values *append(linked_values *pStart, int value)
{
    if (pStart == NULL)
    {
        return pStart;
    }

    linked_values *ptr = pStart;
    while (ptr->next != NULL)
    {
        ptr = ptr->next;
    }

    linked_values *new;

    ptr->next = new;

    return;
} */

void add_node(t_pointer *parent, int value, int *bi)
{
    if (!(*parent))
    {
        *bi = 1;
        if (!(*parent = (t_pointer)malloc(sizeof(node))))
        {
            perror("MALLOC: Failed to reserve memory for a node. Exiting with code 1.");
            exit(1);
        }
        (*parent)->left = (*parent)->right = NULL;
        (*parent)->balance = 0;
        (*parent)->value = value;
    }
    else if (value < (*parent)->value)
    {
        add_node(&(*parent)->left, value, bi);
        if (*bi)
        {
            switch ((*parent)->balance)
            {
            case -1:
                (*parent)->balance = 0;
                *bi = 0;
                break;
            case 0:
                (*parent)->balance = 1;
                break;
            case 1:
                rotate_left(parent, bi);
            }
        }
    }
    else if (value > (*parent)->value)
    {
        add_node(&(*parent)->right, value, bi);
        if (*bi)
        {
            switch ((*parent)->balance)
            {
            case 1:
                (*parent)->balance = 0;
                *bi = 0;
                break;
            case 0:
                (*parent)->balance = -1;
                break;
            case -1:
                rotate_right(parent, bi);
            }
        }
    }
    else
    {
        *bi = 0;
        printf("Value %d already exists.\n", value);
    }
}

/* int calc_depth(t_pointer node, int value)
{
    int i = 0;
    while (node->value != value)
    {
        if (value < node->value)
        {
            node = node->left;
        }
        else if (value > node->value)
        {
            node = node->right;
        }
        else if (value == node->value)
        {
            return i;
            printf("Depth %d", i);
        }
        i++;
    }

    return -1;
} */

void print_tree(t_pointer node, int height)
{
    if (!node)
    {
        return;
    }
    height++;

    print_tree(node->right, height);

    for (int i = 0; i < height; i++)
    {
        printf("    ");
    }

    printf("%d\n", node->value);
    print_tree(node->left, height);
}

void rotate_left(t_pointer *parent, int *bi)
{
    t_pointer child, grandchild;

    child = (*parent)->left;
    if (child->balance == 1) /* LL */
    {
        (*parent)->left = child->right;
        child->right = *parent;
        (*parent)->balance = 0;
        (*parent) = child;
    }
    else /* LR */
    {
        grandchild = child->right;
        child->right = grandchild->left;
        grandchild->left = child;
        (*parent)->left = grandchild->right;
        grandchild->right = *parent;
        switch (grandchild->balance)
        {
        case 1:
            (*parent)->balance = -1;
            child->balance = 0;
            break;
        case 0:
            (*parent)->balance = child->balance = 0;
            break;
        case -1:
            (*parent)->balance = 0;
            child->balance = 1;
        }
        *parent = grandchild;
    }
    (*parent)->balance = 0;
    *bi = 0;
}

void rotate_right(t_pointer *parent, int *bi)
{
    t_pointer child, grandchild;

    child = (*parent)->right;
    if (child->balance == -1) /* RR */
    {
        (*parent)->right = child->left;
        child->left = *parent;
        (*parent)->balance = 0;
        (*parent) = child;
    }
    else /* RL */
    {
        grandchild = child->left;
        child->left = grandchild->right;
        grandchild->right = child;
        (*parent)->right = grandchild->left;
        grandchild->left = *parent;
        switch (grandchild->balance)
        {
        case 1:
            (*parent)->balance = -1;
            child->balance = 0;
            break;
        case 0:
            (*parent)->balance = child->balance = 0;
            break;
        case -1:
            (*parent)->balance = 0;
            child->balance = 1;
        }
        *parent = grandchild;
    }
    (*parent)->balance = 0;
    *bi = 0;
}