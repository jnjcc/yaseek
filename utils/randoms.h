// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef UTILS_RANDOMS_H_
#define UTILS_RANDOMS_H_

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <vector>

namespace yaseek {

// random int from [0, POPULATION)
// NOTICE: thread-unsafe due to rand()
inline int randint(int population) {
  if (population <= 0) {
    return 0;
  }
  return (int)(1.0 * population * rand() / (RAND_MAX + 1.0));
}
// random int from [MIN, MAX)
inline int randint(int min, int max) {
  int size = -1; 
  if (min > max) {
    size = min; min = max; max = size;
  }
  size = max - min;
  return (min + randint(size));
}

// Reservoir sampling with probability NSAMPLE / n
int Reservoir(FILE *fin, std::vector<std::string> *pool, int nsample,
    int maxline = 1024);
int Reservoir(FILE *fin, FILE *fout, int nsample, int maxline = 1024);
// Reservoir sampling with C++ fstream
int Reservoir(std::istream &is, std::vector<std::string> *pool, int nsample);
int Reservoir(std::istream &is, std::ostream &os, int nsample);

}  // namespace yaseek

#endif  // UTILS_RANDOMS_H_
