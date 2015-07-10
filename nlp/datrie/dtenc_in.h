// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef NLP_DATRIE_DTENC_IN_H_
#define NLP_DATRIE_DTENC_IN_H_

// Double-Array Trie encoding, for internal usage only

#include "datrie.h"
#include <stdlib.h>

int setchrset(const char *chrset);
// WCLEN: without the extra 0
wchar_t *dec_word(const char *word, encmode_t enc, int *wclen);
int wstrcmp(const void *lhs, const void *rhs);
wchar_t *wstrncpy(wchar_t *pdest, const wchar_t *psrc, size_t sze);

#endif  // NLP_DATRIE_DTENC_IN_H_
