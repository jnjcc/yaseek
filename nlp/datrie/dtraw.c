// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#include "dtds_in.h"
#include <string.h>

// raw trie generation

static void sort_words(datrie_t *pdt) {
    int nwords = 0, nuniq = 0;
    int *idxs = dhsorti(pdt->pwarry_, &nwords, wstrcmp);

    darray *pcln = dclone(pdt->pwarry_);
    pcln = dclear(pcln);

    // remove duplicate words
    wchar_t lastbuf[MAX_WORD_ARR] = L"";
    const wchar_t *curbuf = NULL;
    uint32_t curlen = 0;
    int i = 0;
    for (i = 0; i < nwords; ++i) {
        curbuf = (const wchar_t *)dgetp(pdt->pwarry_, idxs[i]);
        curlen = dgetlen(pdt->pwarry_, idxs[i]);
        if (wstrcmp(curbuf, lastbuf) == 0) {
            continue;
        } else {
            wstrncpy(lastbuf, curbuf, MAX_WORD_LEN);
        }
        dpush(pcln, curbuf, curlen);
        if (pdt->peidxs_) {
            uint32_t eidx = (uint32_t)idxs[i];  // index into extra info array
            dpush(pdt->peidxs_, &eidx, sizeof(eidx));
        }
        ++nuniq;
    }
    pdt->pwarry_ = drefer(pdt->pwarry_, pcln);
    pdt->word_num_ = nuniq;

    free(idxs);
    free(pcln);
}

static int set_codemap(datrie_t *pdt, wchar_t chr, uint32_t map) {
    if (chr >= pdt->cdmap_sze_) {
        uint32_t diff = chr - pdt->cdmap_sze_ + 1;
        uint32_t nsze = pdt->cdmap_sze_ + diff;
        pdt->code_maps_ = realloc(pdt->code_maps_, nsze * sizeof(uint32_t));
        if (pdt->code_maps_ == NULL) {
            return 0;
        }
        memset(pdt->code_maps_ + pdt->cdmap_sze_, 0, sizeof(uint32_t) * diff);
        pdt->cdmap_sze_ = nsze;
    }
    if (pdt->code_maps_[chr] == 0) {
        pdt->code_maps_[chr] = map;
        return 1;
    }
    return 0;
}

// raw trie tree with siblings sorted
static int build_ord_trie(datrie_t *pdt) {
    const siblings *pnull = NULL;
    ordtree *pord_tree = pdt->ord_tree_;
    // the first level: siblings of root node
    if (dpush(pord_tree, &pnull, sizeof(siblings *)) == 0) {
        return 0;
    }

    wchar_t totmchr = 0;  // total max char code
    uint32_t totmap = 1;  // total code map
    int totlvl = 1;  // total level in ordered tree
    siblings *pcsibs = NULL;  // current level

    uint32_t widx = 0;  // word indx
    uint32_t totword = 0;
    const uint32_t NON_WORD = 0;
    while (widx < dsize(pdt->pwarry_)) {
        const wchar_t *pcurwd = (const wchar_t *)dgetp(pdt->pwarry_, widx);

        wchar_t curchr = 0;
        int curlvl = 0, nxtlvl = 0;
        uint32_t lvlidx = 0;  // sibling_t index
        pcsibs = NULL;

        // existing level
        while (pcurwd && *pcurwd) {
            dget(pord_tree, curlvl, &pcsibs, sizeof(siblings *));
            if (pcsibs) {
                curchr = *pcurwd++;
                if (curchr > totmchr) {
                    totmchr = curchr;
                }
                if (set_codemap(pdt, curchr, totmap)) {
                    ++totmap;
                }

                nxtlvl = find_sib(pcsibs, curchr, pdt->code_maps_, &lvlidx);
                if (nxtlvl != -1) {
                    if (!(*pcurwd)) {  // end of word
                        pcsibs->sibs_[lvlidx].leaf_ = 1;
                        ++totword;
                        // NOTICE: impossible...
                        dset(pdt->pn2wid_, curlvl, &totword, sizeof(totword));
                        break;
                    } else {
                        curlvl = nxtlvl;
                    }
                } else {
                    uint8_t leaf = (*pcurwd) ? 0 : 1;
                    add_sib(pcsibs, lvlidx, curchr, leaf, totlvl);
                    if (leaf) {
                        ++totword;
                        dpush(pdt->pn2wid_, &totword, sizeof(totword));
                    } else {
                        dpush(pdt->pn2wid_, &NON_WORD, sizeof(NON_WORD));
                    }

                    curlvl = totlvl;
                    if (dpush(pord_tree, &pnull, sizeof(siblings *)) == 0) {
                        return 0;
                    }
                    ++totlvl;
                    pcsibs = NULL;
                    break;
                }
            } else {
                break;
            }
        }

        // new level
        while (pcurwd && *pcurwd) {
            curchr = *pcurwd++;
            pcsibs = (siblings *)init_sibs(16);
            if (pcsibs == NULL) {
                return 0;
            }
            dset(pord_tree, curlvl, &pcsibs, sizeof(siblings *));

            uint8_t leaf = *pcurwd ? 0 : 1;
            add_sib(pcsibs, 0, curchr, leaf, totlvl);
            if (leaf) {
                ++totword;
                dpush(pdt->pn2wid_, &totword, sizeof(totword));
            } else {
                dpush(pdt->pn2wid_, &NON_WORD, sizeof(NON_WORD));
            }

            curlvl = totlvl;
            if (dpush(pord_tree, &pnull, sizeof(siblings *)) == 0) {
                return 0;
            }
            ++totlvl;
            if (curchr > totmchr) {
                totmchr = curchr;
            }
            if (set_codemap(pdt, curchr, totmap)) {
                ++totmap;
            }
        }
        ++widx;
    }
    pdt->cdmap_sze_ = totmchr + 1;

    if (!dpush(pdt->pn2wid_, &NON_WORD, sizeof(NON_WORD))) {
        return 0;
    }

    dfree(pdt->pwarry_);
    pdt->pwarry_ = NULL;
    return 1;
}

int build_raw_trie(datrie *pvdt) {
    datrie_t *pdt = (datrie_t *)pvdt;

    pdt->cdmap_sze_ = CODE_MAP_SIZE;
    pdt->code_maps_ = (uint32_t *)calloc(CODE_MAP_SIZE, sizeof(uint32_t));
    pdt->ord_tree_ = dslloc(sizeof(siblings *) * 1024, sizeof(siblings *));
    pdt->pn2wid_ = dslloc(sizeof(uint32_t) * 1024, sizeof(uint32_t));

    if (pdt->code_maps_ == NULL || pdt->ord_tree_ == NULL ||
        pdt->pn2wid_ == NULL) {
        return 0;
    }

    sort_words(pdt);
    return build_ord_trie(pdt);
}

int get_raw_trie(const datrie *pvdt, const char *word, void *extra,
        uint32_t maxsz) {
    datrie_t *pdt = (datrie_t *)pvdt;
    int wlen = 0, widx = 0;
    wchar_t *wstr = dec_word(word, ENC_UTF8, &wlen);
    ordtree *pord_tree = pdt->ord_tree_;
    siblings *pcsibs = NULL;
    int curlvl = 0, nxtlvl = 0;
    uint32_t lvlidx = 0;
    while (widx < wlen) {
        wchar_t curchr = wstr[widx++];
        dget(pord_tree, curlvl, &pcsibs, sizeof(siblings *));
        if (pcsibs) {
            nxtlvl = find_sib(pcsibs, curchr, pdt->code_maps_, &lvlidx);
            if (nxtlvl != -1) {
                curlvl = nxtlvl;
            } else {
                break;
            }
        } else {
            break;
        }
    }
    if (wlen > 0 && widx == wlen) {
        if (extra != NULL) {
            uint32_t widx;
            // the index is the child's level in ord_tree_, a.k.a, NXTLVL
            dget(pdt->pn2wid_, nxtlvl - 1, &widx, sizeof(widx));
            uint32_t eidx;
            dget(pdt->peidxs_, widx - 1, &eidx, sizeof(eidx));
            dget(pdt->pextra_, eidx, extra, maxsz);
        }
        return 1;
    } else {
        return 0;
    }
}

int find_raw_trie(const datrie *pdt, const char *word) {
    return get_raw_trie(pdt, word, NULL, 0);
}
