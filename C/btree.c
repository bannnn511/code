/*
Resources:
    -
https://github.com/kevin-wayne/algs4/blob/master/src/main/java/edu/princeton/cs/algs4/BTree.java#L203
    - https://eatonphil.com/btrees.html
 */

#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// max children per B-tree node = M-1
#define M 4

typedef struct entry {
    char *key;
    char *value;
    struct node *next;
} entry;

typedef struct node {
    int n; // number of children
    entry *children[M];
} node;

typedef struct btree {
    node *root; // root of the B-tree
    int height; // height of the B-tree
    int n; // number of key-value pairs in the B-tree
} btree;

static entry *init_entry(entry *e, char *key, char *value, node *next) {
    e->key = key;
    e->value = value;
    e->next = next;

    return e;
}

static node *init_node(node *n, int k) {
    for (int i = 0; i < M; i++) {
        entry *e = malloc(sizeof(entry));
        if (e == NULL) {
            handle_error(errno, "malloc");
        }
        init_entry(e, NULL, NULL, NULL);
        n->children[i] = e;
    }
    n->n = k;

    return n;
}

static void free_node(node *n) {
    for (int i = 0; i < n->n; i++) {
        free(n->children[i]);
    }
    free(n);
}

static void init_btree(btree *bt) {
    node *root = malloc(sizeof(node));
    if (root == NULL) {
        handle_error(errno, "malloc");
    }

    bt->root = init_node(root, 0);
}

static void free_btree(btree *bt) {
    free_node(bt->root);
    free(bt);
}

static char *search(node *n, char *key, int ht) {
    entry *children[M];
    for (int i = 0; i < M; i++) {
        children[i] = n->children[i];
    }

    if (ht == 0) {
        for (int i = 0; i < n->n; i++) {
            if (strcmp(key, children[i]->key) == 0) {
                return children[i]->value;
            }
        }
    } else {
        for (int i = 0; i < n->n; i++) {
            if (i + 1 == n->n || strcmp(key, children[i + 1]->key) < 0) {
                return search(children[i]->next, key, ht - 1);
            }
        }
    }

    return NULL;
}

char *get(btree *bt, char *key) { return search(bt->root, key, bt->height); }

static node *split(node *n) {
    node *h = malloc(sizeof(node));
    if (h == NULL)
        handle_error(errno, "malloc");
    n->n = M / 2;
    init_node(h, M / 2);
    for (int j = 0; j < h->n; j++) {
        h->children[j] = n->children[M / 2 + j];
    }

    return h;
}

static node *insert(node *h, char *key, char *value, int ht) {
    int j;
    entry *e = malloc(sizeof(entry));
    if (e == NULL) {
        handle_error(errno, "malloc");
    }
    init_entry(e, key, value, NULL);

    if (ht == 0) {
        for (j = 0; j < h->n; j++) {
            int cmp = strcmp(key, h->children[j]->key);
            if (cmp < 0)
                break;
        }
    } else {
        for (j = 0; j < h->n; j++) {
            if (j + 1 == h->n || strcmp(key, h->children[j + 1]->key) < 0) {
                node *u = malloc(sizeof(node));
                if (u == NULL)
                    handle_error(errno, "malloc");

                u = insert(h->children[j++]->next, key, value, ht - 1);
                if (u == NULL)
                    return NULL;

                e->key = u->children[0]->key;
                e->next = u;
                break;
            }
        }
    }

    for (int i = h->n; i > j; i--)
        h->children[i] = h->children[i - 1];

    h->children[j] = e;
    h->n++;
    if (h->n < M) {
        return NULL;
    }

    return split(h);
}

void put(btree *bt, char *key, char *value) {
    if (key == NULL) {
        return;
    }

    node *newnode = insert(bt->root, key, value, bt->height);
    bt->n++;
    if (newnode == NULL)
        return;

    node *t = malloc(sizeof(node));
    init_node(t, 2);
    entry *entry1 = malloc(sizeof(entry));
    if (entry1 == NULL) {
        handle_error(errno, "malloc");
    }
    init_entry(entry1, bt->root->children[0]->key, value, bt->root);
    t->children[0] = entry1;

    entry *entry2 = malloc(sizeof(entry));
    if (entry2 == NULL) {
        handle_error(errno, "malloc");
    }

    init_entry(entry2, newnode->children[0]->key, value, newnode);
    t->children[1] = entry2;

    bt->root = t;
    bt->height++;
}

static char *to_string(node *h, int ht, char *indent) {
    char *s = malloc(1024 * sizeof(char));
    entry *children[M];
    for (size_t i = 0; i < M; i++) {
        children[i] = h->children[i];
    }

    if (ht == 0) {
        for (int j = 0; j < h->n; j++) {
            strcat(s, indent);
            strcat(s, children[j]->key);
            strcat(s, " ");
            if (children[j]->value == NULL) {
                strcat(s, "null");
            } else {
                strcat(s, children[j]->value);
            }
            strcat(s, "\n");
        }
    } else {
        for (int j = 0; j < h->n; j++) {
            if (j > 0) {
                strcat(s, indent);
                strcat(s, "(");
                strcat(s, children[j]->key);
                strcat(s, ")\n");
            }
            char *intentCp = malloc(1024 * sizeof(char));
            strcpy(intentCp, indent);
            strcat(intentCp, "     ");
            strcat(s, to_string(children[j]->next, ht - 1, intentCp));
        }
    }
    return s;
}

char *tree_to_string(btree t) { return to_string(t.root, t.height, " "); }

int main(int argc, char *argv[]) {
    btree *bt = malloc(sizeof(btree));
    if (bt == NULL) {
        handle_error(errno, "malloc");
    }

    init_btree(bt);
    put(bt, "www.cs.princeton.edu", "128.112.136.12");
    put(bt, "www.cs.princeton.edu", "128.112.136.11");
    put(bt, "www.cs.princeton.edu", NULL);
    put(bt, "www.princeton.edu", "128.112.128.15");
    put(bt, "www.yale.edu", "130.132.143.21");
    put(bt, "www.simpsons.com", "209.052.165.60");
    put(bt, "www.apple.com", "17.112.152.32");
    put(bt, "www.amazon.com", "207.171.182.16");
    put(bt, "www.ebay.com", "66.135.192.87");
    put(bt, "www.cnn.com", "64.236.16.20");
    put(bt, "www.google.com", "216.239.41.99");
    put(bt, "www.nytimes.com", "199.239.136.200");
    put(bt, "www.microsoft.com", "207.126.99.140");
    put(bt, "www.dell.com", "143.166.224.230");
    put(bt, "www.slashdot.org", "66.35.250.151");
    put(bt, "www.espn.com", "199.181.135.201");
    put(bt, "www.weather.com", "63.111.66.11");
    put(bt, "www.yahoo.com", "216.109.118.65");

    printf("size:      %d\n", bt->n);
    printf("height:    %d\n", bt->height);
    printf("%s\n", tree_to_string(*bt));

    char *test1 = get(bt, "www.princeton.edu");
    char *test2 = get(bt, "www.espn.com");

    printf("%s\n", test1);
    printf("%s\n", test2);

    free_btree(bt);

    return 0;
}
