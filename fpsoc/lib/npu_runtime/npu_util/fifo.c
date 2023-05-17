#include <stdio.h>
#include <stdlib.h>
#include "npu_defines.h"
#include "fifo.h"


fifo_t* create_fifo(int elem_size, int elem_num)
{
    fifo_t* v = (fifo_t*)malloc(sizeof(fifo_t));

    if (v == NULL)
    {
        return NULL;
    }

    v->elem_num  = elem_num;
    v->elem_size = elem_size;
    v->rdptr     = 0;
    v->wrptr     = 0;
    v->mem       = malloc (elem_num * elem_size);

    return v;
}

int push_fifo_data(fifo_t* v, any_data_t data)
{
    set_fifo_data(v, v->wrptr++, data);

    v->wrptr = v->wrptr == v->elem_num ? 0 : v->wrptr ;

    return v->wrptr;
}

any_data_t pop_fifo_data(fifo_t* v)
{
    any_data_t ret = get_fifo_data(v, v->rdptr++) ;

    v->rdptr = v->rdptr == v->elem_num ? 0 : v->rdptr ;

    return ret ;
}

int set_fifo_data(fifo_t* v, int idx, any_data_t data)
{
    if (idx >= v->elem_num)
        return -1;

    switch (v->elem_size) {
        case 1:
            *((uint8_t*)v->mem + idx) = data.u8[0] ;
            break;
        case 2:
            *((uint16_t*)v->mem + idx) = data.u16[0] ;
            break;
        case 4:
            *((uint32_t*)v->mem + idx) = data.u32[0] ;
            break;
        case 8:
            *((uint64_t*)v->mem + idx) = data.u64 ;
            break;
    }

    return 0;
}

any_data_t get_fifo_data(fifo_t* v, int idx)
{
    any_data_t ret ;
    ret.i64 = -1 ;

    if (idx >= v->elem_num)
        return ret;

    switch (v->elem_size) {
        case 1:
            ret.u8[0] = *((uint8_t*)v->mem + idx) ;
            break;
        case 2:
            ret.u16[0] = *((uint16_t*)v->mem + idx) ;
            break;
        case 4:
            ret.u32[0] = *((uint32_t*)v->mem + idx) ;
            break;
        case 8:
            ret.u64    = *((uint64_t*)v->mem + idx) ;
            break;
    }

    return ret;
}

int fifo_free_room(fifo_t* v)
{
    int ret = v->elem_num - fifo_item_num (v) - 1 ;

    return ret ;
}

int fifo_item_num (fifo_t* v)
{
    int ret ;

    if (v->wrptr >= v->rdptr) {
        ret = v->wrptr - v->rdptr ;
    } else {
        ret = v->elem_num + v->wrptr - v->rdptr ;
    }
    return ret ;
}

int fifo_is_full (fifo_t* v)
{
    if (fifo_free_room (v) <= 1) {
        return 1 ;
    }
    return 0 ;
}
