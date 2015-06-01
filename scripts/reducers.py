## Copyright (c) 2012, 2014 Yste.org
## All Rights Reserved.

### Hadoop reducer programs

## By default, this class implements the word count reducer
## Usage: inherit this class, override:
##   GETKEY, GETVALUE, COLLECT, or RESET, __STR__ if necessary
class ClustReducer:
    def __init__(self):
        self.key = None
    def __nonzero__(self):
        return self.key != None
    def clear(self):
        self.key = None
    def includes(self, line):
        return self.key == self.getkey(line)

    def __str__(self):
        return self.key + "\t" + str(self.stats)
    def reset(self, line):
        self.key = self.getkey(line)
        self.stats = self.getvalue(line)
    def collect(self, line):
        """ collect value of LINE into `self.stats` """
        self.stats += self.getvalue(line)
    def getkey(self, line):
        """ extract and _return_ key from LINE """
        return line.strip("\n")
    def getvalue(self, line):
        """ extract and _return_ value from LINE """
        return 1

## Compute summary statistics of clusters
def aggregate(fin, cluster):
    cluster.clear()
    for line in fin:
        if not cluster.includes(line):
            if cluster:
                print cluster
            cluster.reset(line)
        else:
            cluster.collect(line)
    if cluster:
        print cluster

if __name__ == "__main__":
    import sys
    clust = ClustReducer()
    aggregate(sys.stdin, clust)
