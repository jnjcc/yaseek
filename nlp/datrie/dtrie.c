// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#include "dtds_in.h"
#include "stque_in.h"
#include <string.h>

// final double array trie generation

int build_dtrie(datrie *pvdt) {
    datrie_t *pdt = (datrie_t *)pvdt;
    ordtree *pord_tree = pdt->ord_tree_;
    uint32_t nstates = dsize(pord_tree);
    pdt->da_trie_ = (daelem_t *)calloc(1, sizeof(daelem_t) * nstates);
    if (!pdt->da_trie_) {
        return 0;
    }

    int newbase = 1, curbase = 0, curbound = 0;
    uint32_t maxcdmap = 0;
    pdt->da_trie_[0].check_ = -1;
    pdt->da_trie_[0].base_ = -1;

    // start state in DFA, a.k.a, the parent
    state_t cstat;
    cstat.s_ = 0;  // root node as the start state
    cstat.lvlidx_ = 0;  // siblings index of root node

    const sibling_t *ptmpsib = NULL;
    wchar_t tmpchr = 0;
    siblings *pcsibs = NULL, *pnsibs = NULL;
    stque_t *bfsque = init_que(nstates);
    while (1) {
        pcsibs = NULL;
        pnsibs = NULL;

        dget(pord_tree, cstat.lvlidx_, &pcsibs, sizeof(siblings *));
        curbase = newbase;

        tmpchr = pcsibs->sibs_[pcsibs->num_sibs_ - 1].ccode_;
        maxcdmap = pdt->code_maps_[tmpchr];

        uint32_t newsze = curbound + maxcdmap + 1;
        if (newsze >= nstates) {
            pdt->da_trie_ = (daelem_t *)realloc(pdt->da_trie_,
                    newsze * sizeof(daelem_t));
            memset(&pdt->da_trie_[nstates], 0,
                    (newsze - nstates) * sizeof(daelem_t));
            nstates = newsze;
        }
        if (curbound) {
            int k = 0;
            while (k < pcsibs->num_sibs_) {
                tmpchr = pcsibs->sibs_[k].ccode_;
                uint32_t tcmap = pdt->code_maps_[tmpchr];
                if (pdt->da_trie_[curbase + tcmap].base_) {
                    ++curbase;
                    k = 0;
                } else {
                    ++k;
                }
            }
        }

        // base[s] = CURBASE
        pdt->da_trie_[cstat.s_].base_ = curbase;
        if (cstat.leaf_) {
            pdt->da_trie_[cstat.s_].base_ = curbase | LEAF_FLAG;
        }

        ++newbase;

        int i = 0;
        for (i = 0; i < pcsibs->num_sibs_; ++i) {
            // base[s] + c
            tmpchr = pcsibs->sibs_[i].ccode_;
            int idx = curbase + pdt->code_maps_[tmpchr];
            pdt->da_trie_[idx].base_ = -1;
            // check[base[s] + c] = s
            pdt->da_trie_[idx].check_ = cstat.s_;
            ptmpsib = get_sib(pcsibs, i);

            pnsibs = NULL;
            dget(pord_tree, ptmpsib->chld_, &pnsibs, sizeof(siblings *));
            if (pnsibs) {
                push_state(bfsque, idx, ptmpsib->chld_, ptmpsib->leaf_);
                if (ptmpsib->leaf_) {
                    // LEAF NODE
                }
            } else {
                // LEAF NODE
            }
        }

        if (curbase + maxcdmap > curbound) {
            curbound = curbase + maxcdmap;
        }

        if (!pop_state(bfsque, &cstat)) {
            break;
        }
    }
    free_que(bfsque);

    ++curbound;
    int i = 0;
    for (i = 0; i < curbound; ++i) {
        if (pdt->da_trie_[i].base_ == 0) {
            pdt->da_trie_[i].check_ = -1;
        }
    }

    dfree(pdt->ord_tree_);
    pdt->ord_tree_ = NULL;

    return 1;
}

int find_dtrie(const datrie *pvdt, const char *key,
        void *pval, uint32_t maxsz) {
    const datrie_t *pdt = (const datrie_t *)pvdt;
    int wclen = 0, widx = 0;
    wchar_t *wstr = dec_word(key, pdt->encode_, &wclen);
    if (wclen <= 0) {
        return 0;
    }
    // double array
    int base = 1, check = 0;
    int found = 0;
    int retcode = 0;
    while (widx < wclen) {
        wchar_t curchr = wstr[widx];
        if (curchr >= pdt->cdmap_sze_) {
            break;
        }
        uint32_t cdmap = pdt->code_maps_[curchr];
        // index of base[]
        int bidx = base + cdmap;
        daelem_t *cstat = &(pdt->da_trie_[bidx]);
        if (cstat->check_ != check) {
            break;
        }
        if ((widx == wclen-1) && (cstat->base_ & LEAF_FLAG)) {
            found = 1;
            retcode = 1;
        }
        ++widx;
        base = (int)(cstat->base_ & (~LEAF_FLAG));
        check = bidx;
    }
    if (!found && widx < wclen) {
        // NOTFOUND_ISEND
        retcode = 3;
    }
    return retcode;
}
