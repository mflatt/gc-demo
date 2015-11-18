#include <stdlib.h>
#include "../common/node.h"
#include "../common/mem.h"
#include "allocate.h"

struct node *allocate()
{
  return raw_malloc(sizeof(struct node));
}

void deallocate(struct node *p)
{
  return raw_free(p, sizeof(struct node));
}
