
#define PUSH_STACK_POINTER(var) refcount_inc(var)
#define POP_STACK_POINTER(var)  refcount_dec(var)

/* For a variable that holds the return value: */
#define POP_STACK_RETURN_POINTER(var) refcount_dec_no_free(var)

/* Before setting a field, drop the refcount on the old
   field's vale. After setting a field, increment the
   refcount on the new field value. */
#define SET_NODE(field, val)    (refcount_dec(field), refcount_inc(field = val))

struct node *allocate();
void refcount_inc(struct node *p);
void refcount_dec(struct node *p);
void refcount_dec_no_free(struct node *p);
