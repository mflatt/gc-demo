
/* Does a client need to explicitly deallocate? */
#ifndef MANUAL_DEALLOCATE
# define MANUAL_DEALLOCATE 0
#endif

/* A client uses PUSH_STACK_POINTER() and POP_STACK_POINTER() to
   report when local variable sbecome live and go away,
   respectively. */
#ifndef PUSH_STACK_POINTER
# define PUSH_STACK_POINTER(var) /* nothing */
# define POP_STACK_POINTER(var)  /* nothing */
#endif

/* Use POP_STACK_RETURN_POINTER for a variable that
   holds a function's return address, in case the
   allocator needs to know when a value is being
   returned. */
#ifndef POP_STACK_RETURN_POINTER
# define POP_STACK_RETURN_POINTER(var) POP_STACK_POINTER(var)
#endif

/* Use SET_NODE() to assign to a field, in case the allocator needs to
   know about asignments. */
#ifndef SET_NODE
# define SET_NODE(field, val) field = val
#endif
