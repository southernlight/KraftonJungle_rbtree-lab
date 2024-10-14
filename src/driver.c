#include "rbtree.h"

int main(int argc, char *argv[]) { 

   rbtree* t = new_rbtree();
   node_t* p=rbtree_insert(t,1024);
   printf("%d\n",p->key);
}
