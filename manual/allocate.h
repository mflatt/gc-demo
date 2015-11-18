
#define MANUAL_DEALLOCATE 1
#define PUSH_STACK_POINTER(var) /* do nothing */
#define POP_STACK_POINTER()     /* do nothing */

struct node *allocate();
void deallocate(struct node *p);
