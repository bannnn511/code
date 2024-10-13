#include "util.h"

// max children per B-tree node = M-1
#define M 4
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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


static int* search(btree* bt, char* key, int ht)
{
    entry* children = bt->root->children;

    if (ht == 0)
    {
        for (int i = 0; i < bt->n; i++)
        {
            if (strcmp(children[i].key, key) == 0)
            {
                return &children[i].value;
            }
        }
    }
    else
    {
        for (int i = 0; i < bt->n; i++)
        {
            if (i + 1 == bt->n || strcmp(bt->root->children[i + 1]->key, key) < 0)
            {
                return search(bt, key, ht - 1);
            }
        }
    }

    return NULL;
}

int* get(btree* bt, char* key)
{
    return search(bt, key, bt->height);
}

static node* split(node* n)
{
    node* h = malloc(sizeof(node));
    if (h == NULL)
        handle_error(errno, "malloc");

    h->n = M / 2;
    for (int j = 0; j < h->n; j++)
    {
        h->children[j] = h->children[M / 2 + j];
    }

    return h;
}

static node* insert(node* h, char* key, char* value, int ht)
{
    int j;
    entry* e = malloc(sizeof(entry));
    if (e == NULL)
    {
        handle_error(errno, "malloc");
    }

    if (ht == 0)
    {
        for (j = 0; j < h->n; j++)
        {
            if (strcmp(h->children[j]->key, key) <= 0) break;
        }
    }
    else
    {
        for (j = 0; j < h->n; j++)
        {
            if (j + 1 == h->n || strcmp(h->children[j + 1]->key, key) < 0)
            {
                node* u = malloc(sizeof(node));
                if (u == NULL)
                    handle_error(errno, "malloc");

                u = insert(u, key, value, ht - 1);
                if (u == NULL) return NULL;

                e->key = u->children[0]->key;
                break;
            }
        }
    }

    for (int i = h->n; i > j; i--)
        h->children[i] = h->children[i - 1];

    h->children[j] = e;
    h->n++;
    if (h->n < M)
    {
        return NULL;
    }

    return split(h);
}

void put(btree* bt, char* key, char* value)
{
    if (key == NULL)
    {
        return;
    }

    node* newnode = insert(bt->root, key, value, bt->height);
    bt->n++;
    if (newnode == NULL) return;


    node* t = malloc(sizeof(node));
    init_node(t, 2);
    entry* entry1 = malloc(sizeof(entry));
    if (entry1 == NULL)
    {
        handle_error(errno, "malloc");
    }
    init_entry(entry1, bt->root->children[0]->key, value);
    t->children[0] = entry1;

    entry* entry2 = malloc(sizeof(entry));
    if (entry2 == NULL)
    {
        handle_error(errno, "malloc");
    }
    init_entry(entry2, newnode->children[0]->key, value);
    bt->root = t;
    bt->height++;
}


int main(int argc, char* argv[])
{
    btree* btree = malloc(sizeof(btree));
    if (btree == NULL)
    {
        handle_error(errno, "malloc");
    }

    init_btree(btree);
    put(btree, "www.cs.princeton.edu", "128.112.136.12");
    put(btree, "www.cs.princeton.edu", "128.112.136.11");
    put(btree, "www.cs.princeton.edu", NULL);
    put(btree, "www.princeton.edu", "128.112.128.15");
    put(btree, "www.yale.edu", "130.132.143.21");
    put(btree, "www.simpsons.com", "209.052.165.60");
    put(btree, "www.apple.com", "17.112.152.32");
    put(btree, "www.amazon.com", "207.171.182.16");
    put(btree, "www.ebay.com", "66.135.192.87");
    put(btree, "www.cnn.com", "64.236.16.20");
    put(btree, "www.google.com", "216.239.41.99");
    put(btree, "www.nytimes.com", "199.239.136.200");
    put(btree, "www.microsoft.com", "207.126.99.140");
    put(btree, "www.dell.com", "143.166.224.230");
    put(btree, "www.slashdot.org", "66.35.250.151");
    put(btree, "www.espn.com", "199.181.135.201");
    put(btree, "www.weather.com", "63.111.66.11");
    put(btree, "www.yahoo.com", "216.109.118.65");

    free_btree(btree);
}
