#ifndef __VECTOR__HEADER__
#define __VECTOR__HEADER__
#include "tbl.h"

#define MIN_SIZE 8

#ifndef max 
static int max(int a, int b);
#endif
static int roundup(int n);
static Vector *do_make_vector(int size);
Vector *make_vector();
static void extend(Vector *vec, int delta);
Vector *make_vector1(void *e);
Vector *vec_copy(Vector *src);
void vec_push(Vector *vec, void *elem);
void vec_append(Vector *a, Vector *b);
void *vec_pop(Vector *vec);
void *vec_get(Vector *vec, int index);
void vec_set(Vector *vec, int index, void *val);
void *vec_head(Vector *vec);
void *vec_tail(Vector *vec);
Vector *vec_reverse(Vector *vec);
void *vec_body(Vector *vec);
int vec_len(Vector *vec);

#endif
