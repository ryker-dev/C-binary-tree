/*
Balanced Binary Tree in C
BM40A0301 Tietorakenteet ja algoritmit

Author: Emilio Pizzuti
Date: 12.12.2021

This program uses a modified version of the code given in exercise 7 as a framework.
This file also contains of bits of code previously used in my other programs.
Code is in English for clarity and comaptibility.

USAGE
Files can be parsed on program startup by providing them as an argument:
    .\binarytree.exe "values.txt"
*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 8
#define FILENAME_LENGTH 32

#define DEBUG 0

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
void print_nodes(t_pointer, int);
void rotate_left(t_pointer *, int *);
void read_file(const char *, int *);
int count_lines(const char *);
int menu_handler();
int input_to_int();
int find_value(t_pointer, int);

int main(int argc, char const *argv[])
{
    int bi, i, select, num = 0; // bi = balance indicator
    t_pointer tree = NULL;
    char filename[FILENAME_LENGTH] = "";

    if (argc > 1)
    {
        int values[count_lines(argv[1])];
        read_file(argv[1], values);

        for (i = 0; values[i] != 0; i++)
        {
            add_node(&tree, values[i], &bi);
            print_tree(tree, 0);
        }
    }

    int values[count_lines(filename)];
    while (1)
    {
        select = menu_handler();

        values[count_lines(filename)];
        switch (select)
        {
        case 0:
            /* free memory */
            exit(0);
        case 1:
            printf("\nFilename:");
            fgets(filename, FILENAME_LENGTH, stdin);
            filename[strlen(filename) - 1] = '\0';

            read_file(argv[1], values);

            for (i = 0; values[i] != 0; i++)
            {
                add_node(&tree, values[i], &bi);
            }

            print_tree(tree, 0);

            free(values);
            break;
        case 2:
            printf("Input a value: ");
            num = input_to_int();
            add_node(&tree, num, &bi);
            break;
        case 3:
            printf("Value to search: ");
            num = input_to_int();
            find_value(tree, num);
            break;
        case 4:
            print_tree(tree, 0);
            break;
        default:
            printf("Unknown selection.\n");
            break;
        }
    }
    print_tree(tree, 0);

    return 0;
}

////////////////    INPUT    ////////////////

/* Handles the text menu. Returns a negative integer on error.
    INPUT: - -
    OUTPUT int - selection integer */
int menu_handler()
{
    int selection = -1;

    printf("\n1) Read a file\n");
    printf("2) Input value\n");
    printf("3) Search value\n");
    printf("4) Print tree\n");
    printf("0) Exit\n");
    printf("Action: ");

#if DEBUG
    printf("DEBUG: selection was %d\n", selection);
#endif
    return input_to_int();
}

/* Converts the next input into an integer. Returns a negative integer on error.
    INPUT: - -
    OUTPUT int - integer parsed from the user's input */
int input_to_int()
{
    char input[8];
    fgets(input, 8, stdin);
    input[strlen(input) - 1] = '\0';

    printf("\n");

    /* Count for atoi and its inability to recognise a zero */
    if (!strcmp(input, "0"))
    {
        return 0;
    }

    int select = atoi(input);

    if (select == 0)
    {
        return -1;
    }
}

////////////////    FILE READING    ////////////////

/* Counts the amount of lines in an input file
    INPUT: const char * - Filename as a string
    OUTPUT int - integer parsed from the user's input */
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

/* Counts the amount of lines in an input file
    INPUT:
        0: const char * - Filename as a string
        1: int * - An int array to store the values to
    OUTPUT int - How many lines the files has */
void read_file(const char filename[], int values[])
{
    FILE *fp;
    char line[BUFFER_SIZE];
    int i = 0;

    fp = fopen(filename, "r");
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

////////////////    PRINTING    ////////////////

/* Prints the given tree.
    INPUT:
        0: t_pointer - The root node of the tree
        1: int - A height value required for formatting. Height should always be 0 on first call.
    OUTPUT void */
void print_tree(t_pointer node, int height)
{
    printf("----------------\n");
    print_nodes(node, height);
    printf("----------------\n");
}

void print_nodes(t_pointer node, int height)
{
    if (!node)
    {
        return;
    }
    height++;

    print_nodes(node->right, height);

    for (int i = 0; i < height; i++)
    {
        printf("    ");
    }

    printf("%d\n", node->value);
    print_nodes(node->left, height);
}

////////////////    NODE CONTROL    ////////////////

/* Adds a node into the tree an takes care of balancing.
    INPUT:
        0: t_pointer - The parent node the new value should be a child of.
        1: int - The numeric value of the node.
        2: int - The balance index of the whole tree. Passed from main().
    OUTPUT void */
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

/* Conducts both LL and LR rotations.
    INPUT:
        0: t_pointer - The parent node the new value should be a child of.
        1: int - The balance index of the whole tree. Passed from main().
    OUTPUT void */
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

/* Conducts both RR and RL rotations.
    INPUT:
        0: t_pointer - The parent node the new value should be a child of.
        1: int - The balance index of the whole tree. Passed from main().
    OUTPUT void */
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

////////////////    NODE CONTROL    ////////////////

/* Adds a node into the tree an takes care of balancing.
    INPUT:
        0: t_pointer - The parent node the new value should be a child of.
        1: int - The numeric value of the node.
        2: int - The balance index of the whole tree. Passed from main().
    OUTPUT void */
int find_value(t_pointer node, int value)
{

    if (!node)
    {
        printf("Value %d was not found!", value);
        return 0;
    }

    if (value < node->value)
    {
        find_value(node->left, value);
    }
    else if (value > node->value)
    {
        find_value(node->right, value);
    }
    else
    {
        if (value == node->value)
        {
            printf("Value %d was found in the tree!", value);
            return 1;
        }
        else
        {
            printf("Value %d was not found!", value);
            return 1;
        }
    }

    return 0;
}