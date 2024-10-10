#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"

#define RED 0
#define BLACK 1


typedef struct node {
    char *key; // key
    char *val; // associated data
    struct node *left, *right; // subtrees
    int n; // # nodes in this subtree
    int color;  // color of link from parent to this node
} node;

void init_node(node *new, char *key, char *value) {
    new->key = key;
    new->val = value;
}

typedef struct rbtree {
    node *root;
} rbtree;

void print(rbtree *rbt, node *n, void (*print_func)(void *), int depth, char *label);


static int size(node *n) {
    if (n == NULL) {
        return 0;
    }

    return n->n;
}

int is_red(node *n) {
    if (n == NULL) {
        return 0;
    }

    return n->color == RED;
}

static node *rotate_left(node *n) {
    if (n == NULL) {
        return NULL;
    }

    node *x = n->right;
    n->right = x->left;
    x->left = n;
    x->color = n->color;
    x->n = n->n;
    n->n = 1 + size(n->left) + size(n->right);

    return x;
}


static node *rotate_right(node *n) {
    if (n == NULL) {
        return NULL;
    }

    node *x = n->left;
    n->left = x->right;
    x->right = n;
    x->color = n->color;
    n->color = RED;
    n->n = 1 + size(n->left) + size(n->right);

    return x;
}

static node *min(node *x) {
    if (x == NULL)
        return NULL;

    if (x->left == NULL)
        return x;

    return min(x->left);
}

static node *max(node *x) {
    if (x == NULL)
        return NULL;
    if (x->right == NULL)
        return x;

    return max(x->right);
}

static void flip_colors(node *n) {
    n->color = RED;
    n->left->color = BLACK;
    n->right->color = BLACK;
}

static node *put(node *n, char *key, char *val) {
    if (key == NULL || val == NULL) {
        return NULL;
    }

    if (n == NULL) {
        node *new = malloc(sizeof(node));
        if (new == NULL) {
            handle_error(errno, "malloc");
        }
        init_node(new, key, val);
        return new;
    }

    int cmp = strcmp(key, n->key);
    if (cmp < 0) {
        node *new = put(n->left, key, val);
        n->left = new;
    } else if (cmp > 0) {
        node *new = put(n->right, key, val);
        n->right = new;
    } else {
        if (is_red(n->right) && !is_red(n->left))
            n = rotate_left(n);

        if (is_red(n->left) && is_red(n->left->left))
            n = rotate_right(n);

        if (is_red(n->right) && is_red(n->left))
            flip_colors(n);
    }

    return n;
}

void tree_print(struct node *x) {
    if (x != NULL) {
        tree_print(x->left);
        printf("%s\t", x->key);
        tree_print(x->right);
    }
}/*
 * put searches for key. Update value if found; grow table if new.
 */
static void tree_put(rbtree *r, char *key, char *val) {
    if (r == NULL)
        return;
    r->root = put(r->root, key, val);
    if (r->root == NULL)
        return;
    r->root->color = BLACK;
}

int main(int argc, char *argv[]) {
    rbtree *tree = malloc(sizeof(rbtree));

    tree_put(tree, "S", "1");
    tree_put(tree, "E", "2");
    tree_put(tree, "A", "3");
    tree_put(tree, "R", "4");
    tree_put(tree, "C", "5");

    tree_print(tree->root);
    free(tree);
    return 0;
}
