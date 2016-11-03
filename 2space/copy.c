#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>
#include <string.h>

#include "../common/mem.h"
#include "../common/node.h"
#include "allocate.h"
#include "copy.h"

/* Current allocation region: */
uintptr_t space_start, space_end;
/* Current allocation pointer: */
uintptr_t space_next;

struct node *allocate()
{
  if (space_next + sizeof(struct gc_node) > space_end)
    collect_garbage();

  struct gc_node *p = (struct gc_node *)space_next;
  space_next += sizeof(struct gc_node);

  p->forwarded = 0;

  return &p->n;
}

/* Registered _addresses_ of local variables that hold pointers, so
   that we can update each local variable: */
struct node **root_addrs[128];
int num_roots;

/* First argument is object to mark/copy, second argument is the
   address of an allocation pointer into the new space */
static void copy_or_forward(struct node **addr,
                            struct gc_node **alloc_pos_ptr)
{
  if (is_allocated(*addr)) {
    struct gc_node *nh;
    nh = NODE_TO_GC(*addr);
    
    if (nh->forwarded) {
      /* Object already copied */
      *addr = nh->forward_to;
    } else {
      /* Allocate a copy: */
      struct gc_node *nh2;
      nh2 = *alloc_pos_ptr;
      (*alloc_pos_ptr)++;
      memcpy(nh2, nh, sizeof(struct gc_node));

      /* Mark old location as copied and set forwarding pointer */
      nh->forwarded = 1;
      nh->forward_to = &nh2->n;
      *addr = &nh2->n;
    }
  }
}

static void traverse_copied(struct gc_node *traverse_ptr,
                            struct gc_node **alloc_pos_ptr)
{
  /* Iterate through new space to copy : */
  while (traverse_ptr < *alloc_pos_ptr) {
    copy_or_forward(&traverse_ptr->n.left, alloc_pos_ptr);
    copy_or_forward(&traverse_ptr->n.right, alloc_pos_ptr);
    traverse_ptr++;
  }
}

/* Argument is the address of a pointer into the new space: */
void copy_from_roots(struct gc_node **alloc_pos_ptr)
{
  struct gc_node *init_ptr = *alloc_pos_ptr;
  
  for (int i = 0; i < num_roots; i++) {
    copy_or_forward(root_addrs[i], alloc_pos_ptr);
  }

  traverse_copied(init_ptr, alloc_pos_ptr);
}
