// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#include "dtenc_in.h"
#include <locale.h>
#include <string.h>

int setchrset(const char *chrset) {
    if (chrset && setlocale(LC_CTYPE, chrset)) {
        return 1;
    } else {
        return 0;
    }
}

// WCLEN: without the extra 0
wchar_t *dec_word(const char *word, encmode_t enc, int *wclen) {
    wchar_t *pwret = NULL;
    *wclen = 0;
    if (enc == ENC_UTF8) {
        int sze = mbstowcs(NULL, word, 0);
        pwret = (wchar_t *)calloc(1, sizeof(wchar_t) * (sze + 1));
        *wclen = mbstowcs(pwret, word, sze);
    } else if (enc == ENC_GBK) {
        int slen = strlen(word);
        pwret = (wchar_t *)calloc(1, sizeof(wchar_t) * (slen + 1));
        char dbytes[2];
        unsigned short sdw = 0;
        int i;
        for (i = 0; i < slen; ++i) {
            dbytes[0] = word[i];
            if ((unsigned char)(word[i]) >= 0x80 && (i+1 < slen) &&
                (word[i+1] != 0)) {
                dbytes[1] = word[++i];
            } else {
                dbytes[1] = 0;
            }
            memcpy(&sdw, dbytes, 2);
            pwret[*wclen] = (wchar_t)sdw;
            *wclen += 1;
        }
    }
    return pwret;
}

int wstrcmp(const void *lhs, const void *rhs) {
    const wchar_t *wls = (const wchar_t *)lhs;
    const wchar_t *wrs = (const wchar_t *)rhs;
    int i = 0;
    for (i = 0; wls[i] != 0 && wrs[i] != 0; ++i) {
        if (wls[i] < wrs[i]) {
            return -1;
        } else if (wls[i] > wrs[i]) {
            return +1;
        }
    }
    if (wls[i] < wrs[i]) {
        return -1;
    } else if (wls[i] > wrs[i]) {
        return +1;
    } else {
        return 0;
    }
}

wchar_t *wstrncpy(wchar_t *pdest, const wchar_t *psrc, size_t sz) {
    int i = 0;
    for (i = 0; i < sz && psrc[i] != 0; ++i) {
        pdest[i] = psrc[i];
    }
    pdest[i] = 0;
    return pdest;
}
