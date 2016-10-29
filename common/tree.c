/* A main program that to be compiled and linked with different
   allocators. */

#include <stdlib.h>
#include <stdio.h>
#include "node.h"
#include "mem.h"

/* This header will be allocator-specific, as selected by 
   `cc -I...`: */
#include "allocate.h"

/* Defaults for macros not defined by "allocate.h" */
#include "default.h"

static void destroy_tree(struct node *n);

static int create_garbage; /* a flag for make_tree() */

/* Synthetic workload for the allocator: create a binary treee at the
   given depth. If `create_garbage` is true, create and discard trees
   along the way. */
static struct node *make_tree(int depth)
{
  if (depth == 0)
    return NULL;
  else {
    if (create_garbage) {
      /* create and destroy a junk branch */
      struct node *n = make_tree(depth-1);
      PUSH_STACK_POINTER(n);
      destroy_tree(n);
      POP_STACK_POINTER(n);
    }

    /* Left subtree */
    struct node *l = make_tree(depth-1);
    PUSH_STACK_POINTER(l);
    
    /* Right subtree */
    struct node *r = make_tree(depth-1);
    PUSH_STACK_POINTER(r);
    
    /* New node */
    struct node *t = allocate();
    PUSH_STACK_POINTER(t);
    SET_NODE(t->left, l);
    SET_NODE(t->right, r);

    POP_STACK_POINTER(r);
    POP_STACK_POINTER(l);
    POP_STACK_RETURN_POINTER(t);

    return t;
  }
}

/* To free the memory of a tree, when explicit deallocation is
   needed: */
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

/* Count tree nodes, which we use as a sanity check on the result
   trees: */
static int count(struct node *n)
{
  if (n == NULL)
    return 0;
  else
    return 1 + count(n->left) + count(n->right);
}

/* This program makes some trees, counts their nodes, 
   reports peak memory use as recorded in "mem.c", and
   exits: */
int main(int argc, char **argv)
{
  /* create one tree without intermediate garbage: */
  struct node *init_n = make_tree(10);
  PUSH_STACK_POINTER(init_n);

  /* create a larger tree with lots of intermediate garbage: */
  create_garbage = 1;
  struct node *n = make_tree(16);
  PUSH_STACK_POINTER(n);

  /* check the trees: */
  printf("%d\n", count(n) + count(init_n));
  
  destroy_tree(n);
  destroy_tree(init_n);

  POP_STACK_POINTER(n);
  POP_STACK_POINTER(init_n);

  printf("Peak memory use: %ld\n", peak_mem_use);

  return 0;
}
