
#define PUSH_STACK_POINTER(var) root_addrs[num_roots++] = &var
#define POP_STACK_POINTER(var)  --num_roots

/* Since a GC moves allocated objects, we need variable addresses o be
   registered, not just the value of each variable */
extern struct node **root_addrs[];
extern int num_roots;

struct node *allocate();
