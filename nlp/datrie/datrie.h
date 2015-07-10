// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef NLP_DATRIE_DATRIE_H_
#define NLP_DATRIE_DATRIE_H_

// Double-Array Trie

#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ENC_UTF8 = 0, ENC_GBK = 1
} encmode_t;

typedef void datrie;

datrie *dtalloc(encmode_t encode, const char *chrset, uint64_t init, int extra);
void dtfree(datrie *pdt);
uint32_t dtsize(const datrie *pdt);
int dtadd(datrie *pdt, const char *word, const void *wextra, uint32_t exlen);

int dtsave(const datrie *pdt, const char *fpath);
datrie *dtload(const char *fpath);

int dtbuild(datrie *pdt);
int dtfind(const datrie *pdt, const char *key, void *pval, uint32_t maxsz);

int build_raw_trie(datrie *pdt);
int find_raw_trie(const datrie *pdt, const char *word);

#ifdef __cplusplus
}
#endif

#endif  // NLP_DATRIE_DATRIE_H_
