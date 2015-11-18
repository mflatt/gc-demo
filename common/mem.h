extern size_t mem_use, peak_mem_use;

void *raw_malloc(size_t s);
void raw_free(void *p, size_t s);
