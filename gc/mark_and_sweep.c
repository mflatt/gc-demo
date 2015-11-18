#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>

#include "../common/node.h"
#include "../common/mem.h"
#include "allocate.h"
#include "mark_and_sweep.h"

static void get_more_memory();
struct node_plus_header *free_list = NULL;

struct node *allocate()
{
  struct node_plus_header *nh;
  
  if (!free_list) {
    collect_garbage();
    while (!free_list)
      get_more_memory();
  }

  nh = free_list;
  free_list = free_list->next_free;

  return &nh->n;
}

int num_roots = 0;
struct node *roots[128];

int num_chunks, num_chunked_nodes, num_free_nodes;

struct gc_chunk *chunks = NULL;

static void get_more_memory()
{
  int i;
  struct gc_chunk *c;
  struct node_plus_header *nh;

  // printf("Allocate more\n");

  c = raw_malloc(sizeof(struct gc_chunk));
  c->next = chunks;
  chunks = c;
  num_chunks++;

  c->mem = raw_malloc(NODES_PER_CHUNK * sizeof(struct node_plus_header));

  /* Add everything in the new chunk to the free list: */
  for (i = 0; i < NODES_PER_CHUNK; i++) {
    nh = c->mem + i;
    nh->next_free = free_list;
    free_list = nh;
  }

  num_chunked_nodes += NODES_PER_CHUNK;
  num_free_nodes += NODES_PER_CHUNK;
}

void mark(struct node *n)
{
  if (n != NULL) {
    struct node_plus_header *nh;
    nh = (struct node_plus_header *)((char *)n - offsetof(struct node_plus_header, n));
    
    if (!nh->marked) {
      nh->marked = 1;
      mark(nh->n.left);
      mark(nh->n.right);
    }
  }
}

void mark_and_sweep_from_roots()
{
  int i;
  struct gc_chunk *c;

  // printf("Collect garbage\n");

  /* reset all marks */
  for (c = chunks; c; c = c->next) {
    for (i = 0; i < NODES_PER_CHUNK; i++) {
      c->mem[i].marked = 0;
    }
  }

  /* mark (i.e., find all referenced) */
  for (i = 0; i < num_roots; i++) {
    mark(roots[i]);
  }

  /* sweep (i.e., add unmarked to the free list) */
  free_list = NULL;
  for (c = chunks; c; c = c->next) {
    c->num_marked_nodes = 0;
    for (i = 0; i < NODES_PER_CHUNK; i++) {
      if (!c->mem[i].marked) {
        c->mem[i].next_free = free_list;
        free_list = &c->mem[i];
        num_free_nodes++;
      } else
        c->num_marked_nodes++;
    }
  }
}
