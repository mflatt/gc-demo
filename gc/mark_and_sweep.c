#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "../common/node.h"
#include "../common/mem.h"
#include "allocate.h"
#include "mark_and_sweep.h"

static void get_more_memory();
struct gc_node *free_list = NULL;

struct node *allocate()
{
  struct gc_node *nh;
  
  if (!free_list) {
    collect_garbage();
    if (!free_list)
      get_more_memory();
  }

  nh = free_list;
  free_list = free_list->next_free;

  return &nh->n;
}

/* Hold roots registered by the main program: */
int num_roots = 0;
struct node **root_addrs[128];

/* Allocation statistics: */
int num_chunks, num_chunked_nodes, num_free_nodes;

/* A linked list of alloction chunks: */
struct gc_chunk *chunks = NULL;

/* When a GC doesn't free up any memory, allocate another chunk */
static void get_more_memory()
{
  // printf("Allocate more\n");

  struct gc_chunk *c = raw_malloc(sizeof(struct gc_chunk));
  c->next = chunks;
  chunks = c;
  num_chunks++;

  c->mem = raw_malloc(NODES_PER_CHUNK * sizeof(struct gc_node));

  /* Add everything in the new chunk to the free list: */
  for (int i = 0; i < NODES_PER_CHUNK; i++) {
    struct gc_node *nh = c->mem + i;
    nh->next_free = free_list;
    free_list = nh;
  }

  num_chunked_nodes += NODES_PER_CHUNK;
  num_free_nodes += NODES_PER_CHUNK;
}

/* A helper for mark_and_sweep_from_roots() */
static void mark(struct node *n)
{
  if (n != NULL) {
    struct gc_node *nh;
    nh = NODE_TO_GC(n);
    
    if (!nh->marked) {
      nh->marked = 1;

      /* Recur to keep referenced objects live: */
      mark(nh->n.left);
      mark(nh->n.right);
    }
  }
}

/* Also a helper for mark_and_sweep_from_roots() */
static void reset_all_marks()
{
  for (struct gc_chunk *c = chunks; c; c = c->next) {
    for (int i = 0; i < NODES_PER_CHUNK; i++) {
      c->mem[i].marked = 0;
    }
  }
}

static void sweep()
{
  free_list = NULL;
  for (struct gc_chunk *c = chunks; c; c = c->next) {
    c->num_marked_nodes = 0;
    for (int i = 0; i < NODES_PER_CHUNK; i++) {
      if (!c->mem[i].marked) {
        c->mem[i].next_free = free_list;
        free_list = &c->mem[i];
        num_free_nodes++;
      } else
        c->num_marked_nodes++;
    }
  }
}

/* A garbage_collect() function calls this one */
void mark_and_sweep_from_roots()
{
  // printf("Collect garbage\n");

  reset_all_marks();

  /* mark (i.e., find all referenced) */
  for (int i = 0; i < num_roots; i++)
    mark(*(root_addrs[i]));

  /* sweep (i.e., add unmarked to the free list) */
  sweep();
}
