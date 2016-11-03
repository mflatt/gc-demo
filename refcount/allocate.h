
/* For a variable that is going out of scope: */
#define UNSET_NODE(var)       refcount_dec(var)

/* For a variable that holds the return value: */
#define UNSET_RETURN_NODE(var) refcount_dec_no_free(var)

/* Before setting a variable field, drop the refcount on the old
   value. After setting a variable or field, increment the refcount on
   the new value. */
#define SET_NODE(var, val)    (refcount_dec(var), refcount_inc(var = val))

struct node *allocate();
void refcount_inc(struct node *p);
void refcount_dec(struct node *p);
void refcount_dec_no_free(struct node *p);
