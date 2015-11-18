#include <stdlib.h>
#include <inttypes.h>

#include "../common/node.h"
#include "../common/mem.h"
#include "copy.h"

static uintptr_t prev_size = 1024;

void collect_garbage()
{
  size_t new_size;
  void *new_space;
  struct node_plus_header *alloc_pos;

  /* Set up new space */
  new_size = (space_end - space_start + sizeof(struct node_plus_header));
  if (new_size < (prev_size * 2))
    new_size = prev_size * 2;
  
  new_space = raw_malloc(new_size);
  alloc_pos = new_space;

  /* Copy into new space */
  copy_from_roots(&alloc_pos);

  /* Clean up old space */
  if (space_start != 0)
    raw_free((void *)space_start, space_end - space_start);

  /* Continue allocating into new space */
  space_start = (uintptr_t)new_space;
  space_end = space_start + new_size;
  space_next = (uintptr_t)alloc_pos;

  /* Remember size after collection to be used as a guide for the next
     space's size: */
  prev_size = space_next - space_start;
}

/* Called by copy_from_roots(): */
int is_allocated(void *p)
{
  /* The pointer `p` refers to an allocated object
     if it's in the allocation range: */
  return ((uintptr_t)p >= space_start
          && (uintptr_t)p < space_end);
}
