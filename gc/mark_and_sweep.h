/* Every allocated object needs a header for
   the GC to use: */
struct gc_node {
  union {
    int marked;
    struct gc_node *next_free;
  };
  struct node n;
};

#define NODE_TO_GC(p) ((struct gc_node *)((char *)(p) - offsetof(struct gc_node, n)))

/* List of available nodes for allocation: */
extern struct gc_node *free_list;

extern int num_free_nodes;

/* An allocation chunk is a block of memory
   where we allocate nodes; we keep a linked
   list of chunks */

#define NODES_PER_CHUNK (32 * 1024)

struct gc_chunk {
  struct gc_chunk *next;
  struct gc_node *mem;
  int num_marked_nodes;
};

extern struct gc_chunk *chunks;
extern int num_chunks, num_chunked_nodes;

/* Called from allocate() */
void collect_garbage();

/* To be called by garbage_collect(): */
void mark_and_sweep_from_roots();
