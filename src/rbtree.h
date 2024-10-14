#ifndef _RBTREE_H_
#define _RBTREE_H_

#include <stddef.h>
#include <stdio.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } color_t;

typedef int key_t;

typedef struct node_t {
  color_t color;
  key_t key;
  struct node_t *parent, *left, *right;
} node_t;

typedef struct {
  node_t *root;
  node_t *nil; // for sentinel
} rbtree;

void left_rotate(rbtree *t, node_t *x);
void right_rotate(rbtree *t, node_t *x);

rbtree *new_rbtree(void);
void remove_subtrees(rbtree* t,node_t *p);
void delete_rbtree(rbtree *);

void rbtree_insert_fixup(rbtree *t, node_t*z);
node_t *rbtree_insert(rbtree *, const key_t);
node_t *rbtree_find(const rbtree *, const key_t);
node_t *rbtree_min(const rbtree *);
node_t *rbtree_max(const rbtree *);
void rb_transplant(rbtree *t, node_t *u, node_t* v);
int rbtree_erase(rbtree *, node_t *);
int rbtree_to_array(const rbtree *, key_t *, const size_t);

#endif // _RBTREE_H_
