#include <stdlib.h>
#include <stdio.h>

#include "node.h"
#include "mem.h"
#include "allocate.h"

static void destroy_tree(struct node *n);

int create_garbage;

static struct node *make_tree(int depth)
{
  if (depth == 0)
    return NULL;
  else {
    if (create_garbage) {
      /* create and destroy a junk branch */
      destroy_tree(make_tree(depth-1));
    }

    struct node *l = make_tree(depth-1);
    PUSH_STACK_POINTER(l);
    
    struct node *r = make_tree(depth-1);
    PUSH_STACK_POINTER(r);

    struct node *t = allocate();
    t->left = l;
    t->right = r;

    POP_STACK_POINTER();
    POP_STACK_POINTER();

    return t;
  }
}

static void destroy_tree(struct node *n)
{
#if MANUAL_DEALLOCATE
  if (n != NULL) {
    destroy_tree(n->left);
    destroy_tree(n->right);
    deallocate(n);
  }
#endif
}

static int count(struct node *n)
{
  if (n == NULL)
    return 0;
  else
    return 1 + count(n->left) + count(n->right);
}

int main(int argc, char **argv)
{
  struct node *init_n, *n;
  
  init_n = make_tree(10);
  PUSH_STACK_POINTER(init_n);

  create_garbage = 1;
  n = make_tree(16);

  printf("%d\n", count(n) + count(init_n));
  
  destroy_tree(n);
  destroy_tree(init_n);

  printf("Peak memory use: %ld\n", peak_mem_use);

  return 0;
}
