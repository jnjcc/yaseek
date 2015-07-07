// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#include "darray.h"
#include "dads_in.h"

#include <stdlib.h>
#include <string.h>

// make sure enough space
static int ensure_buf(darray *pvda, uint32_t delta) {
    darray_t *pda = (darray_t *)pvda;
    while ((pda->mem_used_ + delta) > pda->mem_max_) {
        uint64_t nsze = pda->mem_max_ + pda->mem_inc_;
        pda->pdbuf_ = realloc(pda->pdbuf_, nsze);
        if (pda->pdbuf_) {
            pda->mem_max_ = nsze;
        } else {
            return 0;
        }
    }
    return 1;
}
// elements positions
static int ensure_epos(darray *pvda) {
    darray_t *pda = (darray_t *)pvda;
    if (pda->mode_ == DARRAY_FIX || pda->elem_num_ < pda->elem_max_) {
        return 1;
    }
    uint32_t nelem = pda->elem_max_ + pda->elem_inc_;
    pda->pem_sze_ = (uint32_t *)realloc(pda->pem_sze_,
            sizeof(uint32_t) * nelem);
    pda->pem_pos_ = (uint64_t *)realloc(pda->pem_pos_,
            sizeof(uint64_t) * nelem);
    if (pda->pem_sze_ && pda->pem_pos_) {
        pda->elem_max_ = nelem;
        return 1;
    }
    return 0;
}

// malloc a dynamic array with INIMEM mem, with ELESZ mem for each element
static darray *dalloc(dmode_t mode, uint64_t initmem, uint32_t elesz) {
    darray_t *ret = NULL;
    if (initmem == 0 || (mode == DARRAY_FIX && elesz == 0)) {
        return ret;
    }
    ret = (darray_t *)calloc(1, sizeof(darray_t));
    if (ret == NULL) {
        return ret;
    }

    ret->mode_ = mode;
    ret->pdbuf_ = calloc(1, initmem);
    if (ret->pdbuf_ == NULL) {
        return NULL;
    }
    ret->mem_max_ = initmem;
    ret->mem_inc_ = initmem;

    switch (ret->mode_) {
        case DARRAY_FIX:
            ret->elem_sze_ = elesz;
            ret->elem_max_ = (initmem + elesz - 1) / elesz;
            break;
        default:  // including DARRAY_DYN
            ret->elem_max_ = initmem / 8;
            if (ret->elem_max_ <= 0) {
                ret->elem_max_ = 8;
            }
            ret->elem_inc_ = ret->elem_max_;
            ret->pem_sze_ = calloc(sizeof(uint32_t), ret->elem_max_);
            ret->pem_pos_ = calloc(sizeof(uint64_t), ret->elem_max_);
            break;
    }

    return ret;
}

darray *dslloc(uint64_t initmem, uint32_t elesze) {
    return dalloc(DARRAY_FIX, initmem, elesze);
}

darray *ddlloc(uint64_t initmem) {
    return dalloc(DARRAY_DYN, initmem, 0);
}

void dfree(darray *pvda) {
    darray_t *pda = (darray_t *)pvda;
    if (pda->pem_sze_) {
        free(pda->pem_sze_);
        pda->pem_sze_ = NULL;
    }
    if (pda->pem_pos_) {
        free(pda->pem_pos_);
        pda->pem_pos_ = NULL;
    }
    if (pda->pdbuf_) {
        free(pda->pdbuf_);
        pda->pdbuf_ = NULL;
    }
    free(pda);
}

darray *dclone(const darray *pvda) {
    darray_t *dret = NULL;
    const darray_t *pda = (const darray_t *)pvda;
    if (pda == NULL) {
        return dret;
    }
    dret = (darray_t *)malloc(sizeof(darray_t));
    memcpy(dret, pda, sizeof(darray_t));
    dret->pdbuf_ = malloc(dret->mem_used_);
    memcpy(dret->pdbuf_, pda->pdbuf_, dret->mem_used_);

    if (dret->mode_ == DARRAY_DYN) {
        dret->pem_pos_ = malloc(sizeof(uint64_t) * dret->elem_num_);
        dret->pem_sze_ = malloc(sizeof(uint32_t) * dret->elem_num_);
        memcpy(dret->pem_pos_, pda->pem_pos_,
                sizeof(uint64_t) * dret->elem_num_);
        memcpy(dret->pem_sze_, pda->pem_sze_,
                sizeof(uint32_t) * dret->elem_num_);
    }
    return dret;
}

darray *dclear(darray *pvda) {
    darray_t *pda = (darray_t *)pvda;
    if (pda == NULL) {
        return NULL;
    }
    pda->elem_num_ = 0;
    pda->mem_used_ = 0;
    return pda;
}

darray *drefer(darray *pvdest, const darray *pvsrc) {
    darray_t *pdest = (darray_t *)pvdest;
    const darray_t *psrc = (const darray_t *)pvsrc;
    if (pdest == NULL || psrc == NULL) {
        return 0;
    }
    if (pdest->pem_sze_) {
        free(pdest->pem_sze_);
    }
    if (pdest->pem_pos_) {
        free(pdest->pem_pos_);
    }
    if (pdest->pdbuf_) {
        free(pdest->pdbuf_);
    }
    memcpy(pdest, psrc, sizeof(*psrc));
    return pdest;
}

uint32_t dsize(const darray *pvda) {
    const darray_t *pda = (const darray_t *)pvda;
    return pda->elem_num_;
}

int dpush(darray *pvda, const void *psrc, uint32_t sze) {
    darray_t *pda = (darray_t *)pvda;
    if (!ensure_buf(pda, sze)) {
        return 0;
    }
    memcpy(pda->pdbuf_ + pda->mem_used_, psrc, sze);

    if (pda->mode_ == DARRAY_DYN) {
        if (!ensure_epos(pda)) {
            return 0;
        }
        pda->pem_pos_[pda->elem_num_] = pda->mem_used_;
        pda->pem_sze_[pda->elem_num_] = sze;
    }

    // increase
    pda->mem_used_ += sze;
    pda->elem_num_ += 1;
    return 1;
}

void *dgetp(const darray *pvda, uint32_t idx) {
    const darray_t *pda = (const darray_t *)pvda;
    uint64_t off = 0;
    if (idx >= pda->elem_num_) {
        return NULL;
    }
    switch (pda->mode_) {
        case DARRAY_FIX:
            off = pda->elem_sze_ * idx;
            break;
        default:
            off = pda->pem_pos_[idx];
            break;
    }
    return pda->pdbuf_ + off;
}

uint32_t dgetlen(const darray *pvda, uint32_t idx) {
    const darray_t *pda = (const darray_t *)pvda;
    if (idx >= pda->elem_num_) {
        return 0;
    }
    switch (pda->mode_) {
        case DARRAY_FIX:
            return pda->elem_sze_;
        default:
            return pda->pem_sze_[idx];
    }
}

int dget(const darray *pvda, uint32_t idx, void *pdest, uint32_t maxsz) {
    const darray_t *pda = (const darray_t *)pvda;
    uint64_t off = 0;
    uint32_t elen = 0;
    uint32_t szcpy = 0;
    if (idx >= pda->elem_num_) {
        return 0;
    }
    switch (pda->mode_) {
        case DARRAY_FIX:
            off = pda->elem_sze_ * idx;
            elen = pda->elem_sze_;
            break;
        default:
            off = pda->pem_pos_[idx];
            elen = pda->pem_pos_[idx];
            break;
    }
    szcpy = (elen > maxsz) ? maxsz : elen;
    memcpy(pdest, pda->pdbuf_ + off, szcpy);
    return 1;
}

int dpop(darray *pvda, void *pdest, uint32_t maxsz) {
    darray_t *pda = (darray_t *)pvda;
    int ret = 0;
    if (pda->elem_num_ > 0) {
        ret = dget(pda, pda->elem_num_ - 1, pdest, maxsz);
    }
    if (ret > 0) {
        // decrease
        pda->mem_used_ -= pda->pem_sze_[pda->elem_num_ - 1];
        pda->elem_num_ -= 1;
    }
    return ret;
}

// move memory beginning from IDX-th element
//   DIFF < 0: left; DIFF > 0: right
static int move_buf(darray *pvda, uint32_t idx, int32_t diff) {
    darray_t *pda = (darray_t *)pvda;
    uint32_t absdiff = (diff >= 0) ? diff : (-diff);
    if (diff > 0 && !ensure_buf(pda, diff)) {
        return 0;
    }
    if (idx >= dsize(pda)) {
        return 1;
    }
    uint64_t off = 0;
    if (pda->mode_ == DARRAY_FIX) {
        if ((absdiff % pda->elem_sze_) != 0) {
            return 0;
        }
        off = pda->elem_sze_ * idx;
    } else {
        off = pda->pem_pos_[idx];
        uint32_t i;
        for (i = idx; i < dsize(pda); ++i) {
            pda->pem_pos_[i] += diff;
        }
    }
    uint64_t mvsze = (pda->mem_used_ - off);
    memmove(pda->pdbuf_ + off + diff, pda->pdbuf_ + off, mvsze);
    pda->mem_used_ += diff;
    return 1;
}
static void do_copy(darray *pvda, uint32_t idx, uint64_t off, const void *psrc,
        uint32_t sze) {
    darray_t *pda = (darray_t *)pvda;
    memcpy(pda->pdbuf_ + off, psrc, sze);
    if (pda->mode_ == DARRAY_DYN) {
        pda->pem_sze_[idx] = sze;
        pda->pem_pos_[idx] = off;
    }
}
static int dset_right(darray *pvda, uint32_t idx, const void *psrc,
        uint32_t sze) {
    darray_t *pda = (darray_t *)pvda;
    uint32_t elen = dgetlen(pda, idx);
    uint64_t off = get_offset(pda, idx);
    int32_t diff = (sze - elen);
    if (idx + 1 < dsize(pda)) {
        if (!move_buf(pda, idx+1, diff)) {
            return 0;
        }
    } else {
        if (!ensure_buf(pda, diff)) {
            return 0;
        }
    }

    do_copy(pda, idx, off, psrc, sze);
    return 1;
}
static int dset_left(darray *pvda, uint32_t idx, const void *psrc,
        uint32_t sze) {
    darray_t *pda = (darray_t *)pvda;
    uint32_t elen = dgetlen(pda, idx);
    uint64_t off = get_offset(pda, idx);
    int32_t diff = (int)(elen - sze) * (-1);
    if (idx + 1 < dsize(pda)) {
        if (!move_buf(pda, idx+1, diff)) {
            return 0;
        }
    }

    do_copy(pda, idx, off, psrc, sze);
    return 1;
}
static int dset_in(darray *pvda, uint32_t idx, const void *psrc, uint32_t sze,
        int strict) {
    darray_t *pda = (darray_t *)pvda;
    if (idx >= dsize(pda)) {
        return 0;
    }
    uint32_t elen = dgetlen(pda, idx);
    uint64_t off = get_offset(pda, idx);
    if (elen < sze) {
        return dset_right(pda, idx, psrc, sze);
    } else {
        if (elen > sze && strict) {
            return dset_left(pda, idx, psrc, sze);
        } else {
            do_copy(pda, idx, off, psrc, sze);
        }
    }
    return 1;
}

int dset(darray *pvda, uint32_t idx, const void *psrc, uint32_t sze) {
    return dset_in(pvda, idx, psrc, sze, 0);
}

int dsets(darray *pvda, uint32_t idx, const void *psrc, uint32_t sze) {
    return dset_in(pvda, idx, psrc, sze, 1);
}

int dadd(darray *pvda, uint32_t idx, const void *psrc, uint32_t sze) {
    if (idx >= dsize(pvda)) {
        return dpush(pvda, psrc, sze);
    } else {
        darray_t *pda = (darray_t *)pvda;
        // original offset, might change after MOVE_BUF
        uint64_t off = get_offset(pda, idx);
        if (move_buf(pda, idx, sze)) {  // move buffer rightwards
            if (pda->mode_ == DARRAY_DYN) {
                if (!ensure_epos(pda)) {
                    return 0;
                }
                uint32_t i;
                for (i = dsize(pda); i > idx; --i) {
                    pda->pem_pos_[i] = pda->pem_pos_[i-1];
                    pda->pem_sze_[i] = pda->pem_sze_[i-1];
                }
            }
            pda->elem_num_ += 1;
            do_copy(pda, idx, off, psrc, sze);
            return 1;
        }
    }
    return 0;
}

int ddel(darray *pvda, uint32_t idx) {
    if (idx >= dsize(pvda)) {
        return 0;
    } else {
        int32_t diff = (-1) * (int)(dgetlen(pvda, idx));
        if (move_buf(pvda, idx+1, diff)) {
            darray_t *pda = (darray_t *)pvda;
            if (pda->mode_ == DARRAY_DYN) {
                uint32_t i;
                for (i = idx; i+1 < dsize(pda); ++i) {
                    pda->pem_pos_[i] = pda->pem_pos_[i+1];
                    pda->pem_sze_[i] = pda->pem_sze_[i+1];
                }
            }
            pda->elem_num_ -= 1;
            return 1;
        }
    }
    return 0;
}

static int dsave_in(const darray *pvda, FILE *fout) {
    const darray_t *pda = (const darray_t *)pvda;
    int sze = 0;
    if (pda == NULL || pda->pdbuf_ == NULL || fout == NULL) {
        return sze;
    }
    fwrite(pda, sizeof(*pda), 1, fout);
    fwrite(pda->pdbuf_, pda->mem_used_, 1, fout);
    sze += (sizeof(*pda) + pda->mem_used_);

    if (pda->pem_pos_) {
        fwrite(pda->pem_pos_, pda->elem_num_, sizeof(pda->pem_pos_[0]), fout);
        sze += pda->elem_num_ * sizeof(pda->pem_pos_[0]);
    }
    if (pda->pem_sze_) {
        fwrite(pda->pem_sze_, pda->elem_num_, sizeof(pda->pem_sze_[0]), fout);
        sze += pda->elem_num_ * sizeof(pda->pem_pos_[0]);
    }
    return sze;
}

int dsave(const darray *pda, const char *fpath) {
    FILE *fout = fopen(fpath, "wb");
    if (fout == NULL) {
        return 0;
    } else {
        int ret = dsave_in(pda, fout);
        fclose(fout);
        return ret;
    }
}

static darray *dload_in(FILE *fin) {
    darray_t *dret = NULL;
    if (fin == NULL) {
        return dret;
    }
    dret = (darray_t *)malloc(sizeof(darray_t));
    fread(dret, sizeof(*dret), 1, fin);
    dret->pdbuf_ = malloc(dret->mem_used_);
    fread(dret->pdbuf_, 1, dret->mem_used_, fin);

    if (dret->mode_ == DARRAY_DYN) {
        dret->pem_pos_ = malloc(sizeof(uint64_t) * dret->elem_num_);
        dret->pem_sze_ = malloc(sizeof(uint32_t) * dret->elem_num_);
        fread(dret->pem_pos_, sizeof(dret->pem_pos_[0]), dret->elem_num_, fin);
        fread(dret->pem_sze_, sizeof(dret->pem_sze_[0]), dret->elem_num_, fin);
    }

    return dret;
}

darray *dload(const char *fpath) {
    FILE *fin = fopen(fpath, "rb");
    if (fin == NULL) {
        return NULL;
    } else {
        darray *ret = dload_in(fin);
        fclose(fin);
        return ret;
    }
}

void dwalk(const darray *pvda, void (*visit)(const void *, uint32_t)) {
    const darray_t *pda = (const darray_t *)pvda;
    int i;
    for (i = 0; i < pda->elem_num_; ++i) {
        visit(dgetp(pda, i), dgetlen(pda, i));
    }
}
