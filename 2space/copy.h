/* During a GC, we need to know whether an object has been forwarded
   or not: */
struct gc_node {
  int forwarded;
  union {
    struct node n;
    struct node *forward_to;
  };
};

#define NODE_TO_GC(p) ((struct gc_node *)((char *)(p) - offsetof(struct gc_node, n)))

extern struct node **root_addrs[];
extern int num_roots;

/* Allocation region and current allocation pointer: */
extern uintptr_t space_start, space_end, space_next;

/* Called by garbage_collect(): */
void copy_from_roots(struct gc_node **alloc_pos_ptr);

/* Called by copy_from_roots(): */
int is_allocated(void *p);

/* Called by allocate(): */
void collect_garbage();
