// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.

#include "randoms.h"

namespace yaseek {

int Reservoir(FILE *fin, std::vector<std::string> *pool, int nsample,
    int maxline) {
  pool->clear();
  pool->reserve(nsample);

  char *line = (char *)malloc(sizeof(char) * maxline);
  if (line == NULL) {
    return -1;
  }
  int npopulation = 0;
  while (fgets(line, maxline, fin)) {
    ++npopulation;
    std::string sline(line);
    if (npopulation <= nsample) {
      pool->push_back(sline);
    } else {
      int indx = randint(npopulation);
      if (indx < nsample) {
        (*pool)[indx] = sline;
      }
    }
  }
  return 1;
}
int Reservoir(FILE *fin, FILE *fout, int nsample, int maxline) {
  std::vector<std::string> pool;
  int ret = Reservoir(fin, &pool, nsample, maxline);
  if (ret > 0) {
    std::vector<std::string>::const_iterator cit;
    for (cit = pool.begin(); cit != pool.end(); ++cit) {
      fprintf(fout, "%s", cit->c_str());
    }
  }
  return ret;
}
int Reservoir(std::istream &is, std::vector<std::string> *pool, int nsample) {
  pool->clear();
  pool->reserve(nsample);

  std::string sline;
  int npopulation = 0;
  while (std::getline(is, sline)) {
    ++npopulation;
    if (npopulation <= nsample) {
      pool->push_back(sline);
    } else {
      int indx = randint(npopulation);
      if (indx < nsample) {
        (*pool)[indx] = sline;
      }
    }
  }
  return 1;
}
int Reservoir(std::istream &is, std::ostream &os, int nsample) {
  std::vector<std::string> pool;
  int ret = Reservoir(is, &pool, nsample);
  if (ret > 0) {
    std::vector<std::string>::const_iterator cit;
    for (cit = pool.begin(); cit != pool.end(); ++cit) {
      os << *cit << std::endl;
    }
  }
  return ret;
}

}  // namespace yaseek
