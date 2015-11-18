This code demonstrates basic implementation techniques for garbage
collection. In subdirectories:

 * manual = manual allocion and deallocation (i.e., not GC)
 
 * gc = basic mark-and-sweep with a cooperative C program

 * cgc = conservative mark-and-sweep with an uncooperative C program

 * gengc = generational mark-and-sweep

 * 2space = 2-space copying GC; allocated objects move during a GC
 
 * gen2space = 2-space copying GC

The "common/tree.c" program provides a synthetic workload of
allocating a binary tree up to some depth, optionally creating and
discarding extra trees along the way. The makfile combines that
workload with each of the memory-management implementations.

The "common/mem.c" library provides a wrapper for malloc() and free()
so that peak memory can be reported at the end of the test workload.
Different techniques can have substatially different peak memory
sizes.


For simplicity all allocations are for a single datatype, `struct
node` as declared in "common/mem.h". As a further simplification (for
generational GCs), allocated objects refer only to objects allocated
that were earlier.
