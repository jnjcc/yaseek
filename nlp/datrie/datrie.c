// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#include "dtds_in.h"
#include <stdlib.h>
#include <string.h>

datrie *dtalloc(encmode_t encode, const char * chrset, uint64_t initmem,
        int extra) {
    datrie_t *pret = NULL;
    if (encode != ENC_UTF8 && encode != ENC_GBK) {
        return NULL;
    }
    if (!setchrset(chrset)) {
        return NULL;
    }
    pret = (datrie_t *)calloc(1, sizeof(datrie_t));
    if (pret == NULL) {
        return pret;
    }
    pret->encode_ = encode;
    pret->pwarry_ = ddlloc(initmem);
    pret->word_num_ = 0;

    pret->pextra_ = NULL;
    pret->peidxs_ = NULL;
    if (extra) {
        pret->pextra_ = ddlloc(initmem);
        if (initmem < sizeof(uint32_t)) {
            initmem = sizeof(uint32_t);
        }
        pret->peidxs_ = dslloc(initmem, sizeof(uint32_t));
    }

    if (pret->pwarry_ == NULL) {
        free(pret);
        return NULL;
    }
    if (extra && ((pret->pextra_ == NULL) || (pret->peidxs_ == NULL))) {
        free(pret);
        return NULL;
    }

    return pret;
}

void dtfree(datrie *pvdt) {
    datrie_t *pdt = (datrie_t *)pvdt;
    if (pdt->pwarry_) {
        dfree(pdt->pwarry_);
        pdt->pwarry_ = NULL;
    }
    if (pdt->pextra_) {
        dfree(pdt->pextra_);
        pdt->pextra_ = NULL;
    }
    if (pdt->peidxs_) {
        dfree(pdt->peidxs_);
        pdt->peidxs_ = NULL;
    }
    if (pdt->code_maps_) {
        free(pdt->code_maps_);
        pdt->code_maps_ = NULL;
    }
    if (pdt->ord_tree_) {
        dfree(pdt->ord_tree_);
        pdt->ord_tree_ = NULL;
    }
    if (pdt->da_trie_) {
        free(pdt->da_trie_);
        pdt->da_trie_ = NULL;
    }
    free(pdt);
}

uint32_t dtsize(const datrie *pvdt) {
    const datrie_t *pdt = (const datrie_t *)pvdt;
    return pdt->word_num_;
}

int dtadd(datrie *pvdt, const char *word, const void *wextra, uint32_t exlen) {
    datrie_t *pdt = (datrie_t *)pvdt;
    int wclen = 0;
    wchar_t *wstr = dec_word(word, pdt->encode_, &wclen);
    int ret = dpush(pdt->pwarry_, wstr, sizeof(wchar_t) * (wclen + 1));
    free(wstr);
    if (ret > 0 && pdt->pextra_) {
        if (wextra) {
            ret = dpush(pdt->pextra_, wextra, exlen);
        } else {
            ret = dpush(pdt->pextra_, (const void *)"", 1);
        }
    }
    return ret;
}

int dtbuild(datrie *pvdt) {
    if (build_raw_trie(pvdt)) {
        return build_dtrie(pvdt);
    } else {
        return 0;
    }
}

int dtfind(const datrie *pvdt, const char *key, void *pval, uint32_t maxsz) {
    return find_dtrie(pvdt, key, pval, maxsz);
}
