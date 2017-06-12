#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "v_rbtree.h"


static v_inline void v_rbtree_left_rotate(v_rbtree_node_t **root,
    v_rbtree_node_t *sentinel, v_rbtree_node_t *node);
static v_inline void v_rbtree_right_rotate(v_rbtree_node_t **root,
    v_rbtree_node_t *sentinel, v_rbtree_node_t *node);



void
v_rbtree_insert(v_thread_volatile v_rbtree_t *tree,
    v_rbtree_node_t *node)
{
    v_rbtree_node_t  **root, *temp, *sentinel;

    /* a binary tree insert */

    root = (v_rbtree_node_t **) &tree->root;
    sentinel = tree->sentinel;

    if (*root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        v_rbt_black(node);
        *root = node;

        return;
    }

    tree->insert(*root, node, sentinel);

    /* re-balance tree */

    while (node != *root && v_rbt_is_red(node->parent)) {

        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right;

            if (v_rbt_is_red(temp)) {
                v_rbt_black(node->parent);
                v_rbt_black(temp);
                v_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    v_rbtree_left_rotate(root, sentinel, node);
                }

                v_rbt_black(node->parent);
                v_rbt_red(node->parent->parent);
                v_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } else {
            temp = node->parent->parent->left;

            if (v_rbt_is_red(temp)) {
                v_rbt_black(node->parent);
                v_rbt_black(temp);
                v_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    v_rbtree_right_rotate(root, sentinel, node);
                }

                v_rbt_black(node->parent);
                v_rbt_red(node->parent->parent);
                v_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    v_rbt_black(*root);
}



void
v_rbtree_insert_value(v_rbtree_node_t *temp, v_rbtree_node_t *node,
    v_rbtree_node_t *sentinel)
{
    v_rbtree_node_t  **p;

    for ( ;; ) {

        if (node->key.v < temp->key.v) {
            p = &temp->left;
        }
        else if (node->key.v == temp->key.v && node->key.v2 < temp->key.v2) {
            p = &temp->left;
        }
        else {
            p = &temp->right;
        }
      
        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    v_rbt_red(node);
}


void
v_rbtree_insert_timer_value(v_rbtree_node_t *temp, v_rbtree_node_t *node,
    v_rbtree_node_t *sentinel)
{
    v_rbtree_node_t  **p;

    for ( ;; ) {

        /*
         * Timer values
         * 1) are spread in small range, usually several minutes,
         * 2) and overflow each 49 days, if milliseconds are stored in 32 bits.
         * The comparison takes into account that overflow.
         */

        /*  node->key < temp->key */
        if (node->key.v < temp->key.v) {
            p = &temp->left;
        }
        else if (node->key.v == temp->key.v && node->key.v2 < temp->key.v2) {
            p = &temp->left;
        }
        else {
            p = &temp->right;
        }

        if (*p == sentinel) {
            break;
        }

        temp = *p;
    }

    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    v_rbt_red(node);
}

int v_rbtree_get(v_thread_volatile v_rbtree_t *tree, v_rbtree_key_t  key,
	v_rbtree_node_t *sentinel, v_rbtree_node_t **node)
{
	v_rbtree_node_t			*nn, *parent;
	int							 value;

	nn = tree->root;
	*node = 0;
	while (nn != sentinel){
		parent = nn;

        if (tree->compare) {
            value = tree->compare(&nn->key, &key);
            if (value > 0) {
                nn = nn->left;
            }
            else if (value == 0) {
                *node = nn;
                return v_OK;
            }
            else {
                nn = nn->right;
            }
        }
        else {
            if (nn->key.v > key.v) {
                nn = nn->left;
            }
            else if (nn->key.v == key.v && nn->key.v2 > key.v2) {
                nn = nn->left;
            }
            else if (nn->key.v < key.v) {
                nn = nn->right;
            }
            else if (nn->key.v == key.v && nn->key.v2 < key.v2) {
                nn = nn->right;
            }
            else {
                *node = nn;
                return v_OK;
            }
        }

       
	}

	return v_ERROR;
}

void
v_rbtree_delete(v_thread_volatile v_rbtree_t *tree,
    v_rbtree_node_t *node)
{
    unsigned int         red;
    v_rbtree_node_t  **root, *sentinel, *subst, *temp, *w;

    /* a binary tree delete */

    root = (v_rbtree_node_t **) &tree->root;
    sentinel = tree->sentinel;

    if (node->left == sentinel) {
        temp = node->right;
        subst = node;

    } else if (node->right == sentinel) {
        temp = node->left;
        subst = node;

    } else {
        subst = v_rbtree_min(node->right, sentinel);

        if (subst->left != sentinel) {
            temp = subst->left;
        } else {
            temp = subst->right;
        }
    }

    if (subst == *root) {
        *root = temp;
        v_rbt_black(temp);

        /* DEBUG stuff */
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key.v = 0;
        node->key.v2 = 0;

        return;
    }

    red = v_rbt_is_red(subst);

    if (subst == subst->parent->left) {
        subst->parent->left = temp;

    } else {
        subst->parent->right = temp;
    }

    if (subst == node) {

        temp->parent = subst->parent;

    } else {

        if (subst->parent == node) {
            temp->parent = subst;

        } else {
            temp->parent = subst->parent;
        }

        subst->left = node->left;
        subst->right = node->right;
        subst->parent = node->parent;
        v_rbt_copy_color(subst, node);

        if (node == *root) {
            *root = subst;

        } else {
            if (node == node->parent->left) {
                node->parent->left = subst;
            } else {
                node->parent->right = subst;
            }
        }

        if (subst->left != sentinel) {
            subst->left->parent = subst;
        }

        if (subst->right != sentinel) {
            subst->right->parent = subst;
        }
    }

    /* DEBUG stuff */
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key.v = 0;
    node->key.v2 = 0;

    if (red) {
        return;
    }

    /* a delete fixup */

    while (temp != *root && v_rbt_is_black(temp)) {

        if (temp == temp->parent->left) {
            w = temp->parent->right;

            if (v_rbt_is_red(w)) {
                v_rbt_black(w);
                v_rbt_red(temp->parent);
                v_rbtree_left_rotate(root, sentinel, temp->parent);
                w = temp->parent->right;
            }

            if (v_rbt_is_black(w->left) && v_rbt_is_black(w->right)) {
                v_rbt_red(w);
                temp = temp->parent;

            } else {
                if (v_rbt_is_black(w->right)) {
                    v_rbt_black(w->left);
                    v_rbt_red(w);
                    v_rbtree_right_rotate(root, sentinel, w);
                    w = temp->parent->right;
                }

                v_rbt_copy_color(w, temp->parent);
                v_rbt_black(temp->parent);
                v_rbt_black(w->right);
                v_rbtree_left_rotate(root, sentinel, temp->parent);
                temp = *root;
            }

        } else {
            w = temp->parent->left;

            if (v_rbt_is_red(w)) {
                v_rbt_black(w);
                v_rbt_red(temp->parent);
                v_rbtree_right_rotate(root, sentinel, temp->parent);
                w = temp->parent->left;
            }

            if (v_rbt_is_black(w->left) && v_rbt_is_black(w->right)) {
                v_rbt_red(w);
                temp = temp->parent;

            } else {
                if (v_rbt_is_black(w->left)) {
                    v_rbt_black(w->right);
                    v_rbt_red(w);
                    v_rbtree_left_rotate(root, sentinel, w);
                    w = temp->parent->left;
                }

                v_rbt_copy_color(w, temp->parent);
                v_rbt_black(temp->parent);
                v_rbt_black(w->left);
                v_rbtree_right_rotate(root, sentinel, temp->parent);
                temp = *root;
            }
        }
    }

    v_rbt_black(temp);
}


static v_inline void
v_rbtree_left_rotate(v_rbtree_node_t **root, v_rbtree_node_t *sentinel,
    v_rbtree_node_t *node)
{
    v_rbtree_node_t  *temp;

    temp = node->right;
    node->right = temp->left;

    if (temp->left != sentinel) {
        temp->left->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;

    } else if (node == node->parent->left) {
        node->parent->left = temp;

    } else {
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}


static v_inline void
v_rbtree_right_rotate(v_rbtree_node_t **root, v_rbtree_node_t *sentinel,
    v_rbtree_node_t *node)
{
    v_rbtree_node_t  *temp;

    temp = node->left;
    node->left = temp->right;

    if (temp->right != sentinel) {
        temp->right->parent = node;
    }

    temp->parent = node->parent;

    if (node == *root) {
        *root = temp;

    } else if (node == node->parent->right) {
        node->parent->right = temp;

    } else {
        node->parent->left = temp;
    }

    temp->right = node;
    node->parent = temp;
}

