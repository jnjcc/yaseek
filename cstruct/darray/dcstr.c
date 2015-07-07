// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#include "darray.h"
#include <string.h>

int dpushstr(darray *pda, const char *pstr) {
    return dpush(pda, (const void *)pstr, strlen(pstr) + 1);
}

int dsetstr(darray *pda, uint32_t idx, const char *pstr) {
    return dset(pda, idx, (const void *)pstr, strlen(pstr) + 1);
}

int dsetstrs(darray *pda, uint32_t idx, const char *pstr) {
    return dsets(pda, idx, (const void *)pstr, strlen(pstr) + 1);
}

int daddstr(darray *pda, uint32_t idx, const char *pstr) {
    return dadd(pda, idx, (const void *)pstr, strlen(pstr) + 1);
}
