// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.

#include "stque_in.h"
#include <stdlib.h>

stque_t *init_que(int nstate) {
    stque_t *ret = (stque_t *)malloc(sizeof(stque_t));
    if (!ret) {
        return NULL;
    }
    ret->cmem_ = cmlloc(sizeof(state_t) * nstate);
    if (ret->cmem_ == NULL) {
        return NULL;
    }

    ret->head_ = ret->tail_ = NULL;
    ret->free_ = NULL;
    return ret;
}

int push_state(stque_t *sque, int st, int lvlidx, uint8_t leaf) {
    state_t *phead = sque->head_;
    state_t *ptail = sque->tail_;
    state_t *pfree = sque->free_;

    state_t *pnew = NULL;
    if (pfree) {
        pnew = pfree;
        sque->free_ = pfree->next_;
    } else {
        pnew = (state_t *)cmget(sque->cmem_, sizeof(state_t));
        if (pnew == NULL) {
            return 0;
        }
    }
    pnew->s_ = st;
    pnew->lvlidx_ = lvlidx;
    pnew->leaf_ = leaf;
    pnew->next_ = NULL;

    if (phead == NULL) {
        sque->head_ = sque->tail_ = pnew;
    } else {
        ptail->next_ = pnew;
        sque->tail_ = pnew;
    }
    return 1;
}

int pop_state(stque_t *sque, state_t *stat) {
    if (!sque || !stat) {
        return 0;
    }
    state_t *phead = sque->head_;
    state_t *ptail = sque->tail_;
    if (phead) {
        *stat = *(phead);

        state_t *nxt = phead->next_;
        phead->next_ = sque->free_;
        sque->free_ = phead;
        if (phead == ptail) {
            sque->head_ = sque->tail_ = NULL;
        } else {
            sque->head_ = nxt;
        }
        return 1;
    } else {
        return 0;
    }
}

void free_que(stque_t *sque) {
    cmfree(sque->cmem_);
    free(sque);
}
