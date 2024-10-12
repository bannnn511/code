#include "util.h"

// max children per B-tree node = M-1
#define M 4
#include <stdio.h>
#include <stdlib.h>


typedef struct entry
{
    char* key;
    char* value;
} entry;

typedef struct node
{
    int n; // number of children
    entry* children[M];
} node;

typedef struct btree
{
    node* root; // root of the B-tree
    int height; // height of the B-tree
    int n; // number of key-value pairs in the B-tree
} btree;

static entry* init_entry(entry* e, char* key, char* value)
{
    e->key = key;
    e->value = value;
    return e;
}


static node* init_node(node* n, int k)
{
    for (int i = 0; i < k; i++)
    {
        entry* e = malloc(sizeof(entry));
        if (e == NULL)
        {
            handle_error(errno, "malloc");
        }
        init_entry(e, NULL, NULL);
        n->children[i] = e;
    }
    n->n = k;

    return n;
}

static void free_node(node* n)
{
    for (int i = 0; i < n->n; i++)
    {
        free(n->children[i]);
    }
    free(n);
}

static void init_btree(btree* bt)
{
    node* root = malloc(sizeof(node));
    if (root == NULL)
    {
        handle_error(errno, "malloc");
    }

    bt->root = init_node(root, 0);
}

static void free_btree(btree* bt)
{
    free_node(bt->root);
    free(bt);
}


int main(int argc, char* argv[])
{
    btree* bt = malloc(sizeof(btree));
    if (bt == NULL)
    {
        handle_error(errno, "malloc");
    }

    init_btree(bt);
    free_btree(bt);
}
