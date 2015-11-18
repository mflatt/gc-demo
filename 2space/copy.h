
struct node_plus_header {
  int forwarded;
  union {
    struct node n;
    struct node *forward_to;
  };
};

extern struct node **root_addrs[128];
extern int num_roots;

extern uintptr_t space_start, space_end, space_next;

void copy_from_roots(struct node_plus_header **alloc_pos_ptr);

/* Called by copy_from_roots(): */
int is_allocated(void *p);

/* Called by allocate(): */
void collect_garbage();

