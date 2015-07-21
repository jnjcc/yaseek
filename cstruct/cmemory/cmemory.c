// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#include "cmemory.h"
#include "cmds_in.h"
#include <stdlib.h>

cmemory *cmlloc(uint64_t initmem) {
    cmem_t *pmt = (cmem_t *)calloc(1, sizeof(cmem_t));
    if (pmt == NULL) {
        return NULL;
    }
    pmt->pbuf_ = calloc(1, initmem);
    if (pmt->pbuf_ == NULL) {
        free(pmt);
        return NULL;
    }
    pmt->mem_used_ = 0;
    pmt->mem_max_ = pmt->mem_inc_ = initmem;
    return pmt;
}

void *cmget(cmemory *cmry, uint64_t sze) {
    cmem_t *pmt = (cmem_t *)cmry;
    uint64_t nsze = pmt->mem_used_ + sze;
    if (nsze > pmt->mem_max_) {
        if (nsze < pmt->mem_max_ + pmt->mem_inc_) {
            nsze = pmt->mem_max_ + pmt->mem_inc_;
        }
        pmt->pbuf_ = realloc(pmt->pbuf_, nsze);
        if (pmt->pbuf_ == NULL) {
            return NULL;
        }
        pmt->mem_max_ = nsze;
    }
    void *pret = pmt->pbuf_ + pmt->mem_used_;
    pmt->mem_used_ += sze;
    return pret;
}

void cmfree(cmemory *cmry) {
    cmem_t *pmt = (cmem_t *)cmry;
    if (pmt) {
        free(pmt->pbuf_);
        free(pmt);
    }
}
