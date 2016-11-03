
/* Does a client need to explicitly deallocate? */
#ifndef MANUAL_DEALLOCATE
# define MANUAL_DEALLOCATE 0
#endif

/* A client uses PUSH_STACK_POINTER() and POP_STACK_POINTER() to
   report when local variables become live and go away,
   respectively. */
#ifndef PUSH_STACK_POINTER
# define PUSH_STACK_POINTER(var) /* nothing */
# define POP_STACK_POINTER(var)  /* nothing */
#endif

/* Use SET_NODE() to assign to a variable or field, in case the
   allocator needs to know about asignments. Use UNSET_NODE()
   when a variable stops using a pointer because it goes out
   of scope, and use UNSET_RETURN_NODE() for a variable that is
   going out of scope but holds a pointer result (which must
   be immediately claimed by a caller). */
#ifndef SET_NODE
# define SET_NODE(var, val)     var = val
# define UNSET_NODE(var)        /* nothing */
# define UNSET_RETURN_NODE(var) /* nothing */
#endif
