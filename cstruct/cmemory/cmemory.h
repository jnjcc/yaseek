// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef CSTRUCT_CMEMORY_CMEMORY_H_
#define CSTRUCT_CMEMORY_CMEMORY_H_

// Cached Memory

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void cmemory;

cmemory *cmlloc(uint64_t initmem);
void *cmget(cmemory *cmry, uint64_t sze);
void cmfree(cmemory *cmry);

#ifdef __cplusplus
}
#endif

#endif  // CSTRUCT_CMEMORY_CMEMORY_H_
