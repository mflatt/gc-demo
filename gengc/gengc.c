#include <stdlib.h>
#include <stdio.h>

#include "../common/node.h"
#include "../common/mem.h"
#include "../gc/allocate.h"
#include "../gc/mark_and_sweep.h"

/* After a minor collection, move surving chunks into this
   list, which will be ignored until a major collection: */
struct gc_chunk *gen1_chunks;

int num_gen1_chunks, last_major_gen1_chunks;

void collect_garbage()
{
  if (!chunks) return;
  
  /* minor collection: */
  mark_and_sweep_from_roots();

  /* The minor collection above relies on having no references from
     old object to new objects; this is not a general-purpose GC. If
     such references were allowed, we'd have to add them as roots. */
  
  /* move non-empty chunks to old generation: */
  if (chunks) {
    struct gc_chunk *prev_c = NULL, *next;

    for (struct gc_chunk *c = chunks; c; c = next) {
      next = c->next;
      if (c->num_marked_nodes == 0) {
        /* This chunk is empty, so free it */
        raw_free(c->mem, NODES_PER_CHUNK * sizeof(struct gc_node));
        raw_free(c, sizeof(struct gc_chunk));
        if (!prev_c)
          chunks = next;
      } else {
        /* Keep this chunk, now as an old-generation chunk */
        num_gen1_chunks++;
        if (prev_c)
          prev_c->next = c;
        prev_c = c;
      }
      c = next;
    }

    if (prev_c) {
      prev_c->next = gen1_chunks;
      gen1_chunks = chunks;
    }

    chunks = NULL;
    free_list = NULL;
  }

  if (num_gen1_chunks >= (last_major_gen1_chunks * 2)) {
    /* perform major collection by moving all old-generation
       chunks back to the "new" generation, and GC again */
    // printf("Major GC\n");
    last_major_gen1_chunks = num_gen1_chunks;
    chunks = gen1_chunks;
    gen1_chunks = NULL;
    num_gen1_chunks = 0;
    collect_garbage();
  }
}
