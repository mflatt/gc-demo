#include <stdlib.h>
#include <inttypes.h>

#include "../common/node.h"
#include "../common/mem.h"
#include "copy.h"
#include "allocate.h"

#define GEN0_SIZE (4 * 1024)

/* Old-generation region and allocation pointer: */
static uintptr_t gen1_space_start, gen1_space_end, gen1_space_next;

static uintptr_t gen1_prev_size = 1024;

/* Tells is_allocated whether to consider references to the old
   generation: */
static int is_major;

void collect_garbage()
{
  size_t need_size, new_size;
  void *new_space;
  struct node_plus_header *alloc_pos;

  need_size = space_end - space_start;
  if (need_size < (gen1_space_end - gen1_space_next)) {
    /* Minor collection, since we have room in the gen1 space */
    alloc_pos = (struct node_plus_header *)gen1_space_next;

    /* A minor collection (still) relies on having no references from
       old object to new objects; this is not a general-purpose GC. If
       such references were allowed, we'd have to add them as
       roots. */

    is_major = 0;
    copy_from_roots(&alloc_pos);

    /* Record updated allocation pointer */
    gen1_space_next = (uintptr_t)alloc_pos;
  } else {
    /* Major collection; make a space big enough for both the old and
       new generation: */
    size_t new_size = need_size + (gen1_space_next - gen1_space_start);
    if (new_size < (gen1_prev_size * 2))
      new_size = gen1_prev_size * 2;

    new_space = raw_malloc(new_size);
    alloc_pos = (struct node_plus_header *)new_space;
    
    is_major = 1;
    copy_from_roots(&alloc_pos);

    /* Clean up old space */
    if (gen1_space_start != 0)
      raw_free((void *)gen1_space_start, gen1_space_end - gen1_space_start);

    /* Continue allocating into new old-generation space */
    gen1_space_start = (uintptr_t)new_space;
    gen1_space_end = gen1_space_start + new_size;
    gen1_space_next = (uintptr_t)alloc_pos;

    gen1_prev_size = gen1_space_next - gen1_space_start;
  }

  /* Create a fresh new-generation space: */
  new_space = raw_malloc(GEN0_SIZE);

  /* Clean up old new-generation space */
  if (space_start != 0)
    raw_free((void *)space_start, GEN0_SIZE);

  /* Continue allocating into new space */
  space_start = (uintptr_t)new_space;
  space_end = space_start + GEN0_SIZE;
  space_next = space_start;
}

/* used by collect_from_roots: */
int is_allocated(void *p)
{
  return (((uintptr_t)p >= space_start
           && (uintptr_t)p < space_end)
          || (is_major
              /* During a major collection, also treat references
                 into the old generation's space as allocated: */
              && ((uintptr_t)p >= gen1_space_start
                  && (uintptr_t)p < gen1_space_end)));
}
