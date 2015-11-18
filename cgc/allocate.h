
/* Conservative GC means that the program doesn't have to
   cooperate: */
#define MANUAL_DEALLOCATE 0
#define PUSH_STACK_POINTER(var) /* nothing */
#define POP_STACK_POINTER()     /* nothing */

struct node *allocate();

/* Let CGC take over main, so that it can track
   stack positions. */
#define main original_main

