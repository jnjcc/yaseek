// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef NLP_DATRIE_STQUE_IN_H_
#define NLP_DATRIE_STQUE_IN_H_

#include "cmemory/cmemory.h"
#include <stdint.h>

// state in DFA
typedef struct tqnode {
    int s_;  // start state: base[s] + c = t
    int lvlidx_;  // child level index, into ord_tree_
    uint8_t leaf_;
    struct tqnode *next_;
} state_t;

// cached queue
typedef struct {
    cmemory *cmem_;

    state_t *head_;
    state_t *tail_;
    state_t *free_;
} stque_t;

stque_t *init_que(int nstate);
int push_state(stque_t *sque, int st, int lvlidx, uint8_t leaf);
int pop_state(stque_t *sque, state_t *stat);
void free_que(stque_t *sque);

#endif  // NLP_DATRIE_STQUE_IN_H_
