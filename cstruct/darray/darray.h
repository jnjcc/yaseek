// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef CSTRUCT_DARRAY_DARRAY_H_
#define CSTRUCT_DARRAY_DARRAY_H_

// Dynamic Array

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void darray;

darray *dslloc(uint64_t initmem, uint32_t elesze);
darray *ddlloc(uint64_t initmem);
void dfree(darray *pda);

darray *dclone(const darray *pda);
// refer PDEST to PSRC
darray *drefer(darray *pdest, const darray *psrc);
darray *dclear(darray *pda);

uint32_t dsize(const darray *pda);

int dpush(darray *pda, const void *psrc, uint32_t sze);
void *dgetp(const darray *pda, uint32_t idx);
uint32_t dgetlen(const darray *pda, uint32_t idx);
int dget(const darray *pda, uint32_t idx, void *pdest, uint32_t maxsz);
int dpop(darray *pda, void *pdest, uint32_t maxsz);

int dset(darray *pda, uint32_t idx, const void *psrc, uint32_t sze);
int dsets(darray *pda, uint32_t idx, const void *psrc, uint32_t sze);

// add before IDX
int dadd(darray *pda, uint32_t idx, const void *psrc, uint32_t sze);
int ddel(darray *pda, uint32_t idx);

int dsave(const darray *pda, const char *fpath);
darray *dload(const char *fpath);

void dwalk(const darray *pda, void (*visit)(const void *, uint32_t));

int dpushstr(darray *pda, const char *pstr);
int dsetstr(darray *pda, uint32_t idx, const char *pstr);
int dsetstrs(darray *pda, uint32_t idx, const char *pstr);
int daddstr(darray *pda, uint32_t idx, const char *pstr);

// the "index-sort" version
int *dhsorti(darray *pda, int *psze, int (*cmpfn)(const void *, const void *));
int *dqsorti(darray *pda, int *psze, int (*cmpfn)(const void *, const void *));
// "index-sort" with Size
int *dhsortis(darray *pda, int *psze,
    int (*cmpfn)(const void *, uint32_t, const void *, uint32_t));
int *dqsortis(darray *pda, int *psze,
    int (*cmpfn)(const void *, uint32_t, const void *, uint32_t));
// sort-by-index
int dsortidx(darray *pda, int idxs[], int sze);
// the "in-place-sort" version
void dhsort(darray *pda, int (*cmpfn)(const void *, const void *));
void dqsort(darray *pda, int (*cmpfn)(const void *, const void *));
// "in-place-sort" with Size
void dhsorts(darray *pda,
    int (*cmpfn)(const void *, uint32_t, const void *, uint32_t));
void dqsorts(darray *pda,
    int (*cmpfn)(const void *, uint32_t, const void *, uint32_t));

#ifdef __cplusplus
}
#endif

#endif  // CSTRUCT_DARRAY_DARRAY_H_
