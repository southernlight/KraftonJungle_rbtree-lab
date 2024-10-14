#include "rbtree.h"

#include <stdlib.h>

void left_rotate(rbtree *t, node_t *x) {
    node_t *y = x->right;  
    x->right = y->left;    

    if (y->left != t->nil) {
        y->left->parent = x;  
    }

    y->parent = x->parent;  

    if (x->parent == t->nil) {
        t->root = y;  
    } else if (x == x->parent->left) {
        x->parent->left = y;  
    } else {
        x->parent->right = y;  
    }

    y->left = x;  
    x->parent = y;  
}


void right_rotate(rbtree *t, node_t *y) {
    node_t *x = y->left;  
    y->left = x->right;   

    if (x->right != t->nil) {
        x->right->parent = y;  
    }

    x->parent = y->parent;  

    if (y->parent == t->nil) {
        t->root = x;  
    } else if (y == y->parent->right) {
        y->parent->right = x;  
    } else {
        y->parent->left = x;  
    }

    x->right = y;  
    y->parent = x;  
}


rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  return p;
}


void remove_subtrees(rbtree* t , node_t *p)
{
  if(p!=t->nil) 
  {
    remove_subtrees(t,p->left);
    remove_subtrees(t,p->right);
    free(p);
  }
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  remove_subtrees(t,t->root);
  free(t->nil);
  free(t);
}



void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z -> parent -> color == RBTREE_RED) {
    if ((z -> parent) == (z -> parent -> parent -> left)) {
      node_t *y = z -> parent -> parent -> right;
      if (y -> color == RBTREE_RED) {
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }
      else {
        if (z == z -> parent -> right) {
          z = z -> parent;
          left_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        right_rotate(t, z -> parent -> parent);
      }
    }
    else {
    node_t *y = z -> parent -> parent -> left;
      if (y -> color == RBTREE_RED) {
        z -> parent -> color = RBTREE_BLACK;
        y -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        z = z -> parent -> parent;
      }
      else {
        if (z == z -> parent -> left) {
          z = z -> parent;
          right_rotate(t, z);
        }
        z -> parent -> color = RBTREE_BLACK;
        z -> parent -> parent -> color = RBTREE_RED;
        left_rotate(t, z -> parent -> parent);
      }
    }
  }
  t -> root -> color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z -> key = key;
  node_t *x = t -> root;
  node_t *y = y = t -> nil;
  while (x != t -> nil) {
    y = x;
    if ((z -> key) < (x -> key)) {
      x = x -> left;
    } else {
      x = x -> right;
    }
  }
  z -> parent = y;
  if (y == t -> nil) {
    t -> root = z;
  }
  else if ((z -> key) < (y -> key)) {
    y -> left = z;
  }
  else {
    y -> right = z;
  }
  z -> left = t -> nil;
  z -> right = t -> nil;
  z -> color = RBTREE_RED;
  rbtree_insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t* cur = t->root;
  while(cur!=t->nil && cur->key!=key)
  {
    if(cur->key > key)
      cur = cur->left;
    else
      cur = cur->right;
  }
  if(cur!=t->nil)
    return cur;
  else
    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t* min = t->nil;
  node_t* cur = t->root;

  while(cur!=t->nil)
  {
    min = cur;
    cur = cur->left;
  }
  return min;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t* max = t->nil;
  node_t* cur = t->root;

  while(cur!=t->nil)
  {
    max = cur;
    cur = cur->right;
  }
  return max;
}

void rb_transplant(rbtree *t, node_t *u, node_t* v)
{
  if(u->parent==t->nil)
    t->root = v;
  else if(u==u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

node_t* rbtree_submin(rbtree* t , node_t *x)
{
  node_t* successor = t->nil;
  node_t* cur = x;
  while(cur!=t->nil)
  {
    successor = cur;
    cur = cur->left;
  }
  return successor;
}

void rbtree_erase_fixup(rbtree* t, node_t *x)
{
  node_t * w;
  while(x!=t->root && x->color==RBTREE_BLACK)
  {
     if(x==x->parent->left)
     {
      w = x->parent->right;
      // 케이스 1: doubly black의 형제가 red일 때
      if(w->color == RBTREE_RED) 
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t,x->parent);
        w = x->parent->right;
      }
      // 케이스 2: doubly black 의 형제가 black & 그 형제의 두 자녀 모두 black
      else if(w->left->color==RBTREE_BLACK && w->right->color ==RBTREE_BLACK) 
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      else
      {
        // 케이스 3: doubly black의 오른쪽 형제가 black & 그 형제의 왼쪽 자녀가 red & 그 형제의 오른쪽 자녀는 black
        if(w->right->color==RBTREE_BLACK) 
        {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t,w);
          w = x->parent->right;
        }
        // 케이스 4: doubly black의 오른쪽 형제가 black & 그 형제가 오른쪽 자녀가 red일 때
        w->color = x->parent->color;
        w->right->color = RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        left_rotate(t,x->parent);
        x = t->root;
      }

     }
     else
     {
      w = x->parent->left;
      // 케이스 1: doubly black의 형제가 red일 때
      if(w->color == RBTREE_RED) 
      {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t,x->parent);
        w = x->parent->left;
      }
      // 케이스 2: doubly black 의 형제가 black & 그 형제의 두 자녀 모두 black
      else if(w->left->color==RBTREE_BLACK && w->right->color ==RBTREE_BLACK) 
      {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      else
      {
        // 케이스 3: doubly black의 오른쪽 형제가 black & 그 형제의 왼쪽 자녀가 red & 그 형제의 오른쪽 자녀는 black
        if(w->left->color==RBTREE_BLACK) 
        {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t,w);
          w = x->parent->left;
        }
        // 케이스 4: doubly black의 오른쪽 형제가 black & 그 형제가 오른쪽 자녀가 red일 때
        w->color = x->parent->color;
        w->left->color = RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        right_rotate(t,x->parent);
        x = t->root;
      }
     }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase 
  node_t* y = p;
  color_t y_original_color = y->color;
  node_t* x;

  // 왼쪽 자식만 있는 경우
  if(p->left==t->nil)
  {
    x = p->right;
    rb_transplant(t,p,p->right);
  }
  // 오른쪽 자식만 있는 경우
  else if(p->right==t->nil)
  {
    x = p->left;
    rb_transplant(t,p,p->left);
  }
  // 둘다 있는 경우
  else
  {
    y = rbtree_submin(t , p->right);
    y_original_color = y->color;
    x = y->right;
    
    if(y!=p->right)
    {
      rb_transplant(t,y,y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    else
    {
      x->parent = y;
    }
      

    rb_transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;

  }

  if(y_original_color == RBTREE_BLACK)
    rbtree_erase_fixup(t,x);
  free(p);
  return 0;
}

void inorder(const rbtree *t, node_t* cur, int* idx, key_t* arr, const size_t n)
{
  if(cur==t->nil || *idx == n)
    return;
    
  inorder(t,cur->left,idx,arr,n);
  arr[*idx] = cur->key;
  (*idx)++;
  inorder(t,cur->right,idx,arr,n); 
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  int idx=0;
  inorder(t, t->root, &idx, arr, n);
  return 0;
}
