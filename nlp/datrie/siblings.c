// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.

#include "dtds_in.h"

siblings *init_sibs(uint32_t nsibs) {
    siblings *pret = (siblings *)malloc(sizeof(siblings));
    if (pret == NULL) {
        return NULL;
    }
    pret->sibs_ = (sibling_t *)malloc(sizeof(sibling_t) * nsibs);
    if (pret->sibs_ == NULL) {
        free(pret);
        return NULL;
    }
    pret->num_sibs_ = 0;
    pret->max_sibs_ = nsibs;
    return pret;
}

const sibling_t *get_sib(const siblings *psibs, int id) {
    return &(psibs->sibs_[id]);
}

int sib_size(const siblings *psibs) {
    return psibs->num_sibs_;
}

int add_sib(siblings *psibs, int id, wchar_t code, uint8_t leaf, int chld) {
    if (psibs == NULL || id < 0) {
        return 0;
    }
    int mvidx = psibs->num_sibs_;
    ++psibs->num_sibs_;
    if (psibs->num_sibs_ >= psibs->max_sibs_) {
        const int SIB_INC = 8;
        psibs->max_sibs_ += SIB_INC;
        psibs->sibs_ = (sibling_t *)realloc(psibs->sibs_,
                psibs->max_sibs_ * sizeof(sibling_t));
        if (psibs->sibs_ == NULL) {
            return 0;
        }
    }
    while (mvidx > id) {
        psibs->sibs_[mvidx] = psibs->sibs_[mvidx - 1];
        --mvidx;
    }
    psibs->sibs_[id].ccode_ = code;
    psibs->sibs_[id].leaf_ = leaf;
    psibs->sibs_[id].chld_ = chld;
    return 1;
}

int mark_sib_leaf(siblings *psibs, int id) {
    psibs->sibs_[id].leaf_ = 1;
    return 1;
}

int find_sib(const siblings *psibs, wchar_t code, uint32_t *codemap,
        uint32_t *idx) {
    if (psibs == NULL || codemap == NULL || idx == NULL) {
        return 0;
    }
    uint32_t cmap = codemap[code];
    const sibling_t *psb = NULL;

    psb = get_sib(psibs, 0);
    if (cmap == codemap[psb->ccode_]) {
        *idx = 0;
        return psb->chld_;
    } else if (cmap < codemap[psb->ccode_]) {
        *idx = 0;
        return -1;
    } else {
        int n = psibs->num_sibs_ - 1;
        if (n > 0) {
            psb = get_sib(psibs, n);
            if (cmap == codemap[psb->ccode_]) {
                *idx = n;
                return psb->chld_;
            } else if (cmap > codemap[psb->ccode_]) {
                *idx = (n + 1);
                return -1;
            }
        } else {
            *idx = 1;
            return -1;
        }
    }

    int l = 1, r = psibs->num_sibs_ - 2, m = 0;
    while (l <= r) {
        m = l + (r - l) / 2;
        psb = get_sib(psibs, m);
        if (cmap == codemap[psb->ccode_]) {
            *idx = m;
            return psb->chld_;
        } else if (cmap < codemap[psb->ccode_]) {
            r = m - 1;
        } else {
            l = m + 1;
        }
    }

    psb = get_sib(psibs, m);
    if (cmap < codemap[psb->ccode_]) {
        *idx = m;
    } else {
        *idx = (m + 1);
    }
    return -1;
}
