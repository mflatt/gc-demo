
BINS = tree-manual tree-refcount tree-gc tree-cgc tree-gengc tree-2space tree-gen2space

all: $(BINS)

clean:
	rm -rf $(BINS) *.dSYM *~ */*~

COMMON_C = common/tree.c common/mem.c
COMMON_DEP = $(COMMON_C) common/node.h common/mem.h

CFLAGS = -O2 -g

tree-manual: $(COMMON_DEP) manual/allocate.h manual/manual.c
	$(CC) $(CFLAGS) -o tree-manual -Imanual $(COMMON_C) manual/manual.c

tree-refcount: $(COMMON_DEP) refcount/allocate.h refcount/refcount.c
	$(CC) $(CFLAGS) -o tree-refcount -Irefcount $(COMMON_C) refcount/refcount.c

tree-gc: $(COMMON_DEP) gc/allocate.h gc/gc.c gc/mark_and_sweep.c gc/mark_and_sweep.h
	$(CC) $(CFLAGS) -o tree-gc -Igc $(COMMON_C) gc/gc.c gc/mark_and_sweep.c

tree-cgc: $(COMMON_DEP) cgc/allocate.h gc/allocate.h cgc/cgc.c gc/mark_and_sweep.c gc/mark_and_sweep.h
	$(CC) $(CFLAGS) -o tree-cgc -Icgc $(COMMON_C) cgc/cgc.c gc/mark_and_sweep.c

tree-gengc: $(COMMON_DEP) gc/allocate.h gengc/gengc.c gc/mark_and_sweep.c gc/mark_and_sweep.h
	$(CC) $(CFLAGS) -o tree-gengc -Igc $(COMMON_C) gengc/gengc.c gc/mark_and_sweep.c

tree-2space: $(COMMON_DEP) 2space/allocate.h 2space/2space.c 2space/copy.h 2space/copy.c
	$(CC) $(CFLAGS) -o tree-2space -I2space $(COMMON_C) 2space/2space.c 2space/copy.c

tree-gen2space: $(COMMON_DEP) 2space/allocate.h gen2space/gen2space.c 2space/copy.h 2space/copy.c
	$(CC) $(CFLAGS) -o tree-gen2space -I2space $(COMMON_C) gen2space/gen2space.c 2space/copy.c
