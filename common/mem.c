#include <stdlib.h>

#include "mem.h"

size_t mem_use, peak_mem_use;

void *raw_malloc(size_t s)
{
  mem_use += s;
  if (mem_use > peak_mem_use)
    peak_mem_use = mem_use;
  return malloc(s);
}

void raw_free(void *p, size_t s)
{
  mem_use -= s;
  free(p);
}
