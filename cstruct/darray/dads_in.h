// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef CSTRUCT_DARRAY_DADS_IN_H_
#define CSTRUCT_DARRAY_DADS_IN_H_

// Dynamic Array internal data structures

typedef enum {
    DARRAY_FIX = 0, DARRAY_DYN = 1
} dmode_t;

typedef struct {
    void *pdbuf_;
    uint64_t mem_used_;
    uint64_t mem_max_;
    uint64_t mem_inc_;

    uint32_t elem_num_;

    dmode_t mode_;
    uint32_t elem_sze_;

    uint32_t elem_max_;
    uint32_t elem_inc_;
    uint32_t *pem_sze_;
    uint64_t *pem_pos_;
} darray_t;

static inline uint64_t get_offset(const darray_t *pda, uint32_t idx) {
    uint64_t off = 0;
    if (pda->mode_ == DARRAY_FIX) {
        off = pda->elem_sze_ * idx;
    } else {
        off = pda->pem_pos_[idx];
    }
    return off;
}

#endif  // CSTRUCT_DARRAY_DADS_IN_H_
