#include <stdlib.h>
#include <stddef.h>
#include "../common/node.h"
#include "../common/mem.h"
#include "allocate.h"

/* Allocate a counter field with each node: */
struct rc_node {
  int count;
  struct node n;
};

struct node *allocate()
{
  /* A node is alloctaed with a refcount of 0, so the
     client program is responsible for immediately
     incrementing the reference count. */
  struct rc_node *rn = raw_malloc(sizeof(struct rc_node));
  rn->count = 0;
  rn->n.left = NULL;
  rn->n.right = NULL;
  return &rn->n;
}

void refcount_inc(struct node *p)
{
  if (p) {
    /* Increment the reference count */
    struct rc_node *rn;
    rn = (struct rc_node *)((char *)p - offsetof(struct rc_node, n));
    rn->count++;
  }
}

void refcount_dec(struct node *p)
{
  if (p) {
    /* Decrement the reference count, and free if it does to zero */
    struct rc_node *rn;
    rn = (struct rc_node *)((char *)p - offsetof(struct rc_node, n));
    if (--rn->count == 0) {
      refcount_dec(rn->n.left);
      refcount_dec(rn->n.right);
      raw_free(rn, sizeof(struct rc_node));
    }
  }
}

void refcount_dec_no_free(struct node *p)
{
  if (p) {
    /* Decrement the reference count without freeing; used
       to return an allocated object from a function, so
       that the caller can assume ownership. */
    struct rc_node *rn;
    rn = (struct rc_node *)((char *)p - offsetof(struct rc_node, n));
    --rn->count;
  }
}
