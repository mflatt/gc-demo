#include <inttypes.h>

#include "../common/node.h"
#include "../gc/allocate.h"
#include "../gc/mark_and_sweep.h"

extern int original_main(int argc, char **argv);

/* The "noinline" attributes here ensure that stack frames
   are pushed as these functions are called: */
static void find_roots() __attribute__ ((noinline));
static int call_original_main(int argc, char **argv) __attribute__ ((noinline));

static void *stack_init;

void collect_garbage()
{
  find_roots();
  mark_and_sweep_from_roots();
}

int main(int argc, char **argv)
{
  /* Record the original stack position */
  stack_init = &argc;
  
  return call_original_main(argc, argv);
}

int call_original_main(int argc, char **argv)
{
  /* Now that a frame has been pushed, stack variables
     during original_main() are definitely shallower
     than `stack_int` */
  return original_main(argc, argv);
}

static void find_roots()
{
  num_roots = 0;

  /* Since a frame was pushed to call find_roots,
     stack variables during original_main() are definitely
     deeper than `stack_now` */
  void **stack_now = (void **)&stack_now;

  /* Scan the stack for pointers */
  while ((uintptr_t)stack_now < (uintptr_t)stack_init) {
    void *p = *stack_now;

    /* Walk through chunks, checking the pointer range */
    for (struct gc_chunk *c = chunks; c; c = c->next) {
      if (((uintptr_t)p >= (uintptr_t)c->mem)
          && ((uintptr_t)p <= (uintptr_t)(c->mem + NODES_PER_CHUNK))) {
        /* looks like a pointer; round down to `node_plus_header` start
         so that we're looking at the start of the object */
        uintptr_t delta = ((uintptr_t)p - (uintptr_t)c->mem);
        if (delta % sizeof(struct node_plus_header)) {
          delta -= (delta % sizeof(struct node_plus_header));
          p = (void *)((uintptr_t)c->mem + delta);
        }
        roots[num_roots++] = &((struct node_plus_header *)p)->n;
      }
    }
    
    stack_now++;
  }
}
