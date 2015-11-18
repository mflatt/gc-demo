/* Every allocated object needs a header for
   the GC to use: */
struct node_plus_header {
  union {
    int marked;
    struct node_plus_header *next_free;
  };
  struct node n;
};

/* List of available nodes for allocation: */
extern struct node_plus_header *free_list;

extern int num_free_nodes;

/* An allocation chunk is a block of memory
   where we allocate nodes; we keep a linked
   list of chunks */

#define NODES_PER_CHUNK (32 * 1024)

struct gc_chunk {
  struct gc_chunk *next;
  struct node_plus_header *mem;
  int num_marked_nodes;
};

extern struct gc_chunk *chunks;
extern int num_chunks, num_chunked_nodes;

/* Called from allocate() */
void collect_garbage();

/* To be called by garbage_collect(): */
void mark_and_sweep_from_roots();
