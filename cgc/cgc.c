#include <inttypes.h>
#include <stddef.h>
#include <setjmp.h>

#include "../common/node.h"
#include "../gc/allocate.h"
#include "../gc/mark_and_sweep.h"

extern int original_main(int argc, char **argv);

/* The "noinline" attributes here ensure that stack frames
   are pushed as these functions are called: */
static void find_roots(jmp_buf env) __attribute__ ((noinline));
static int call_original_main(int argc, char **argv) __attribute__ ((noinline));

static void *stack_init;

void collect_garbage()
{
  /* Using setjmp() with a stack-allocated `env` ensures that any
    callee-saved register values are moved onto the stack. Pass
    `&env` to fint_roots() just to make sure it stays on the stack.*/
  jmp_buf env;
  if (setjmp(env)) {
    /* Register a stack location as a root when it seems to have a
       pointer value: */
    find_roots(env);
    /* Normal GC: */
    mark_and_sweep_from_roots();
  }
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

static int looks_like_allocated(struct gc_node *p)
{
  /* Walk through chunks, checking the pointer range */
  for (struct gc_chunk *c = chunks; c; c = c->next) {
    if ((p >= c->mem)
        && (p <= (c->mem + NODES_PER_CHUNK))) {
      /* looks like a pointer; make sure it matches a `gc_node`
         start position */
      uintptr_t delta = ((char *)p - (char *)c->mem);
      if ((delta % sizeof(struct gc_node)) == offsetof(struct gc_node, n))
        return 1;
    }
  }

  return 0;
}

static void find_roots(jmp_buf env)
{
  num_roots = 0;

  /* Since a frame was pushed to call find_roots,
     stack variables during original_main() are definitely
     deeper than `stack_now` */
  void **stack_now = (void **)&stack_now;

  /* Scan the stack for pointers */
  while ((void *)stack_now < stack_init) {
    if (looks_like_allocated(*stack_now))
      root_addrs[num_roots++] = (struct node **)stack_now;
    stack_now++;
  }
}
