// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#include "darray.h"
#include "dads_in.h"

#include <stdlib.h>
#include <string.h>

// sort using compare function with SZE

// index of darray
static inline int cmp_ids_sze(darray *pda, int lid, int rid,
        int (*cmpfn)(const void *, uint32_t, const void *, uint32_t)) {
    return cmpfn(dgetp(pda, lid), dgetlen(pda, lid),
                 dgetp(pda, rid), dgetlen(pda, rid));
}
// index of index
static inline int cmp_idx_sze(darray *pda, int idxs[], int lhs, int rhs,
        int (*cmpfn)(const void *, uint32_t, const void *, uint32_t)) {
    int lid = idxs[lhs], rid = idxs[rhs];
    return cmp_ids_sze(pda, lid, rid, cmpfn);
}

static void sift_down_sze(darray *pda, int idxs[], int root, int bot,
        int (*cmpfn)(const void *, uint32_t, const void *, uint32_t)) {
    int mchild = 0;
    while (root * 2 <= bot) {
        if (root * 2 == bot) {
            mchild = root * 2;
        } else {
            if (cmp_idx_sze(pda, idxs, root*2, root*2 + 1, cmpfn) < 0) {
                mchild = root * 2 + 1;
            } else {
                mchild = root * 2;
            }
        }
        if (cmp_idx_sze(pda, idxs, root, mchild, cmpfn) < 0) {
            int tmp = idxs[root];
            idxs[root] = idxs[mchild];
            idxs[mchild] = tmp;
            root = mchild;
        } else {
            break;
        }
    }
}

int *dhsortis(darray *pvd, int *psze,
        int (*cmpfn)(const void *, uint32_t, const void *, uint32_t)) {
    darray_t *pda = (darray_t *)pvd; 
    if (pda->elem_num_ <= 0) {
        return NULL;
    }

    int *idxs = (int *)malloc(sizeof(int) * pda->elem_num_);
    *psze = 0;
    if (idxs == NULL) {
        return NULL;
    }
    int i;
    for (i = 0; i < pda->elem_num_; ++i) {
        idxs[i] = i;
    }

    int sze = pda->elem_num_;
    for (i = (sze / 2); i >= 0; --i) {
        sift_down_sze(pda, idxs, i, sze-1, cmpfn);
    }
    for (i = sze - 1; i >= 1; --i) {
        int temp = idxs[0];
        idxs[0] = idxs[i];
        idxs[i] = temp;
        sift_down_sze(pda, idxs, 0, i-1, cmpfn);
    }

    *psze = pda->elem_num_;
    return idxs;
}

void dhsorts(darray *pvda,
        int (*cmpfn)(const void *, uint32_t, const void *, uint32_t)) {
    darray_t *pda = (darray_t *)pvda; 
    if (pda->elem_num_ <= 1) {
        return;
    }

    int sze = 0;
    int *idxs = dhsortis(pda, &sze, cmpfn);
    if (sze == pda->elem_num_) {
        dsortidx(pda, idxs, pda->elem_num_);
    }
    free(idxs);
}

static void dqsort_in_sze(darray *pda, int idxs[], int low, int high,
        int (*cmpfn)(const void *, uint32_t, const void *, uint32_t)) {
    int i, j, pivt;
    if (low >= high) {
        return;
    }
    i = low; j = high; pivt = idxs[i];
    while (i < j) {
        while (i < j && cmp_ids_sze(pda, idxs[j], pivt, cmpfn) >= 0) {
            --j;
        }
        if (i < j) {
            idxs[i] = idxs[j];
            ++i;
        }

        while (i < j && cmp_ids_sze(pda, idxs[i], pivt, cmpfn) < 0) {
            ++i;
        }
        if (i < j) {
            idxs[j] = idxs[i];
            --j;
        }
    }
    idxs[i] = pivt;
    dqsort_in_sze(pda, idxs, low, i-1, cmpfn);
    dqsort_in_sze(pda, idxs, i+1, high, cmpfn);
}

int *dqsortis(darray *pvd, int *psze,
        int (*cmpfn)(const void *, uint32_t, const void *, uint32_t)) {
    darray_t *pda = (darray_t *)pvd; 
    if (pda->elem_num_ <= 0) {
        return NULL;
    }

    int *idxs = (int *)malloc(sizeof(int) * pda->elem_num_);
    *psze = 0;
    if (idxs == NULL) {
        return NULL;
    }
    int i;
    for (i = 0; i < pda->elem_num_; ++i) {
        idxs[i] = i;
    }
    int sze = pda->elem_num_;
    dqsort_in_sze(pda, idxs, 0, sze - 1, cmpfn);

    *psze = pda->elem_num_;
    return idxs;
}

void dqsorts(darray *pvda,
        int (*cmpfn)(const void *, uint32_t, const void *, uint32_t)) {
    darray_t *pda = (darray_t *)pvda; 
    if (pda->elem_num_ <= 1) {
        return;
    }

    int sze = 0;
    int *idxs = dqsortis(pda, &sze, cmpfn);
    if (sze == pda->elem_num_) {
        dsortidx(pda, idxs, pda->elem_num_);
    }
    free(idxs);
}
