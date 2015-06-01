// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.

#include "configs.h"

namespace yaseek {

ConfigReader::ConfigReader(std::istream &is, const std::string &sep) {
  kvmap_.clear();
  std::string sline;
  while (std::getline(is, sline)) {
    sline = TrimCmnt(sline);
    if (isspaces(sline.c_str())) {
      continue;
    }
    std::vector<std::string> kvpair;
    Split(sline, sep, &kvpair);
    if (kvpair.size() < 2) {
      continue;
    }
    std::string key = Trims(kvpair[0]);
    std::string val = Trims(kvpair[1]);
    kvmap_[key] = val;
  }
}

int ConfigReader::GetConfStr(const std::string &key, std::string *pval) {
  int ret = -1;
  if (kvmap_.find(key) != kvmap_.end()) {
    *pval = kvmap_[key];
    ret = 1;
  }
  return ret;
}

int ConfigReader::GetConfInt(const std::string &key, int *pval) {
  int ret = -1;
  if (kvmap_.find(key) != kvmap_.end()) {
    char *endptr = NULL;
    long lval = strtol(kvmap_[key].c_str(), &endptr, 10);
    *pval = int(lval);
    ret = 1;
  }
  return ret;
}

void ConfigReader::DumpConfigs(std::ostream &os) {
  std::map<std::string, std::string>::const_iterator cit;
  for (cit = kvmap_.begin(); cit != kvmap_.end(); ++cit) {
    os << "<" << cit->first << "> : <" << cit->second << ">" << std::endl;
  }
}

}  // namespace yaseek
