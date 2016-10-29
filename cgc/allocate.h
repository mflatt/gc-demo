
/* Conservative GC means that the program doesn't have to
   cooperate, so all macros are defaults */

struct node *allocate();

/* Let CGC take over main, so that it can track
   stack positions. */
#define main original_main

