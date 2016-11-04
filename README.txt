This code demonstrates basic implementation techniques for garbage
collection. In subdirectories:

 * manual = manual allocation and deallocation (i.e., not GC)

 * refcount = reference counting (also not GC)
 
 * gc = basic mark-and-sweep for a cooperative C program

 * cgc = conservative mark-and-sweep for an uncooperative C program

 * gengc = generational mark-and-sweep

 * 2space = 2-space copying GC; allocated objects move during a GC
 
 * gen2space = generational 2-space copying GC

The "common/tree.c" program provides a synthetic workload of
allocating a binary tree up to some depth, optionally creating and
discarding extra trees along the way. The makfile combines that
workload with each of the memory-management implementations.

The "common/mem.c" library provides a wrapper for malloc() and free()
so that peak memory can be reported at the end of the test workload.
Different techniques can have substatially different peak memory
sizes.


For simplicity all allocations are for a single datatype, `struct
node` as declared in "common/mem.h". The conservative collector does
not support interior pointers (i.e., pointers that are not to the
start of the object). As a further simplification (for generational
GCs), allocated objects refer only to objects allocated that were
earlier.


Videos that walk though parts of the implementation:

 https://www.youtube.com/playlist?list=PLbdXd8eufjyVCrWF-obj8_BbyU5vEF8Jw
