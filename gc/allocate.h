
#define MANUAL_DEALLOCATE 0
#define PUSH_STACK_POINTER(var) roots[num_roots++] = var;
#define POP_STACK_POINTER()     --num_roots

extern struct node *roots[];
extern int num_roots;

struct node *allocate();
