// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef CSTRUCT_CMEMORY_CMDS_IN_H_
#define CSTRUCT_CMEMORY_CMDS_IN_H_

// Cached Memory, the internal data structure

typedef struct {
    void *pbuf_;
    uint64_t mem_used_;
    uint64_t mem_max_;
    uint64_t mem_inc_;
} cmem_t;

#endif  // CSTRUCT_CMEMORY_CMDS_IN_H_
