#ifndef _VEC_H
#define _VEC_H

typedef double data_t;
#define IDENT 1
#define OP *

typedef struct 
{
    long len;
    data_t *data;
} vec_rec, *vec_ptr;

vec_ptr new_vec(long len);
int get_vec_element(vec_ptr v, long index, data_t *dest);
long vec_length(vec_ptr v);
data_t *get_vec_start(vec_ptr v);
void combine1(vec_ptr v, data_t *dest);
void combine2(vec_ptr v, data_t *dest);
void combine3(vec_ptr v, data_t *dest);
void combine4(vec_ptr v, data_t *dest);

#endif