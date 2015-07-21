// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef NLP_DATRIE_DTDS_IN_H_
#define NLP_DATRIE_DTDS_IN_H_

// Double-Array Trie internal data structures
#include "darray/darray.h"
#include "datrie.h"
#include "dtenc_in.h"

#define MAX_WORD_LEN 255
#define MAX_WORD_ARR (MAX_WORD_LEN + 1)
#define MAX_LINE_LEN 1023
#define MAX_LINE_ARR (MAX_LINE_LEN + 1)

#define CODE_MAP_SIZE 65536

#define LEAF_FLAG (1 << 31)

// double array element
typedef struct {
    int base_;
    int check_;
} daelem_t;

typedef darray ordtree;

typedef struct {
    encmode_t encode_;
    // word array
    darray *pwarry_;
    uint32_t word_num_;

    darray *pextra_;  // extra info for unsorted words
    darray *peidxs_;  // n-th word => index in PEXTRA

    // Ordered tree, the raw trie tree
    ordtree *ord_tree_;
    uint32_t *code_maps_;
    uint32_t cdmap_sze_;

    // final double array trie
    daelem_t *da_trie_;
} datrie_t;

// one of siblings
typedef struct {
    wchar_t ccode_;
    uint8_t leaf_;
    int chld_;
} sibling_t;

// sibling_t ordered by codemap
typedef struct {
    sibling_t *sibs_;
    int num_sibs_;
    int max_sibs_;
} siblings;

siblings *init_sibs(uint32_t nsibs);
inline const sibling_t *get_sib(const siblings *psibs, int id);
inline int sib_size(const siblings *psibs);
int add_sib(siblings *psibs, int id, wchar_t code, uint8_t leaf, int chld);
int find_sib(const siblings *psibs, wchar_t code, uint32_t *codemap,
    uint32_t *idx);

int build_dtrie(datrie *pdt);
int find_dtrie(const datrie *pdt, const char *key, void *pval, uint32_t maxsz);

#endif  // NLP_DATRIE_DTDS_IN_H_
