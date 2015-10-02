// NGS objects
#ifndef OBJ_H
#define OBJ_H
#include <stdint.h>
#include <stddef.h>

// On problems with `uintptr_t` change here according to Ruby source in `include/ruby/ruby.h`
// uintptr_t format for printf - PRIXPTR - printf("Blah %" PRIXPTR "\n", VALUE.num);
typedef union value_union {
	uintptr_t num;
	void * ptr;
} VALUE;

typedef struct object_struct {
	VALUE type;
	VALUE val;
} OBJECT;

typedef struct var_len_object_struct {
	OBJECT base;
	size_t len;
} VAR_LEN_OBJECT;

// malloc() / NGS_MALLOC() memory is 8 byte aligned
// .....000 - *OBJECT
// .....001 - int number
// .....010 - false
// .....011 - true
// .....100 - null
// .....101 - reserved
// .....110 - reserved
// .....111 - undef

#define META_BITS    (3)
#define META_AND     (7)
#define META_INT     (1)

#define V_FALSE (2)
#define V_TRUE  (3)
#define V_NULL  (4)
#define V_UNDEF (7)

// object
// .....000 - *TYPE
// .....001 - String
// .....010 - Native method
// .....011 - NGS method
// .....100 - Array
// .....101 - Hash

// TODO: handle situation when n is wider than size_t - META_BITS bits
#define IS_NULL(v)      (v.num == V_NULL)
#define IS_TRUE(v)      (v.num == V_TRUE)
#define IS_FALSE(v)     (v.num == V_FALSE)
#define IS_UNDEF(v)     (v.num == V_UNDEF)
#define IS_NOT_UNDEF(v) (v.num != V_UNDEF)
#define IS_BOOL(v)      ((v.num & 6) == 2)

#define IS_INT(v)    ((v.num & META_AND) == META_INT)
#define SET_INT(v,n) v.num = ((n) << META_BITS) | META_INT
#define MAKE_INT(n)  ((VALUE){.num=((n) << META_BITS) | META_INT})
#define MAKE_BOOL(n)  ((VALUE){.num=(n ? V_TRUE : V_FALSE)})
#define GET_INT(v)   ((v).num >> META_BITS)
#define SET_OBJ(v,o) v.ptr = o
#define SET_OBJECT_TYPE_LSTR(o) o->type.num = 1
#define SET_OBJECT_TYPE_ARRAY(o) o->type.num = 2
#define SET_NULL(v) v.num = V_NULL
#define SET_FALSE(v) v.num = V_FALSE
#define SET_TRUE(v) v.num = V_TRUE
#define SET_UNDEF(v) v.num = V_UNDEF

// TODO: some saner numbering maybe
#define OBJ_TYPE_STRING        (1)
#define OBJ_TYPE_NATIVE_METHOD (2)
#define OBJ_TYPE_USER_METHOD   (3)
#define OBJ_TYPE_ARRAY         (4)

#define OBJ_LEN(v)          ((VAR_LEN_OBJECT *) v.ptr)->len
#define OBJ_DATA_PTR(v)     ((OBJECT *)v.ptr)->val.ptr
#define OBJ_TYPE(v)         (((OBJECT *)v.ptr)->type.num)
#define OBJ_TYPE_PTR(v)     ((OBJECT *)v.ptr)->type.ptr
#define IS_STRING(v)        (((v.num & META_AND) == 0) && OBJ_TYPE(v) == OBJ_TYPE_STRING)
#define IS_NATIVE_METHOD(v) (((v.num & META_AND) == 0) && OBJ_TYPE(v) == OBJ_TYPE_NATIVE_METHOD)
#define IS_USER_METHOD(v)   (((v.num & META_AND) == 0) && OBJ_TYPE(v) == OBJ_TYPE_USER_METHOD)
#define IS_ARRAY(v)         (((v.num & META_AND) == 0) && OBJ_TYPE(v) == OBJ_TYPE_ARRAY)

void dump(VALUE v);
void dump_titled(char *title, VALUE v);
#endif
