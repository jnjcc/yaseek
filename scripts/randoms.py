## Copyright (c) 2012, 2014 Yste.org
## All Rights Reserved.

import random

## random sampling in memory
## NOTICE: will modify LST
def sampling(lst, nsample):
    random.shuffle(lst)
    return lst[0:nsample]

def reservoir(fin, nsample):
    samples = []
    npopulation = 0 
    for line in fin:
        npopulation += 1
        line = line.strip('\n')
        if npopulation <= nsample:
            samples.append(line)
        else:
            rindx = random.randint(0, npopulation)
            if rindx < nsample:
                samples[rindx] = line
    return samples
