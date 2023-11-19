#include <stdio.h>
#include "vec.h"
#include <stdlib.h>

vec_ptr new_vec(long len) {
    vec_ptr result = (vec_ptr) malloc(sizeof(vec_rec));
    data_t *data = NULL;
    if(!result) {
        return NULL;
    }
    result->len = len;
    if(len > 0) {
        data = (data_t *)calloc(len, sizeof(data_t));
        if(!data) {
            free((void *) result);
            return  NULL;
        }
    }

    result->data = data;
    return result;
}

int get_vec_element(vec_ptr v, long index, data_t *dest) {
    if(index < 0 || index >= v->len) {
        return 0;
    }
    *dest = v->data[index];
    return 1;
}


long vec_length(vec_ptr v) {
    return v->len;
}

void combine1(vec_ptr v, data_t *dest) {
    long i;
    *dest = IDENT;
    for(i = 0; i < vec_length(v); ++i) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}

void combine2(vec_ptr v, data_t *dest) {
    long i;
    long length = vec_length(v);

    *dest = IDENT;
    for(i = 0; i < length; ++i) {
        data_t val;
        get_vec_element(v, i, &val);
        *dest = *dest OP val;
    }
}