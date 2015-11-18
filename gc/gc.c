#include "../common/node.h"
#include "allocate.h"
#include "mark_and_sweep.h"

void collect_garbage()
{
  mark_and_sweep_from_roots();
}
