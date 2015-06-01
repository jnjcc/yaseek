// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.
#ifndef UTILS_CONFIGS_H_
#define UTILS_CONFIGS_H_

#include "strings.h"

#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace yaseek {

class ConfigReader {
 public:
  explicit ConfigReader(std::istream &is, const std::string &sep = ":");
  ~ConfigReader() {}
  int GetConfStr(const std::string &key, std::string *pval);
  int GetConfInt(const std::string &key, int *pval);

  void DumpConfigs(std::ostream &os);

 private:
  std::map<std::string, std::string> kvmap_;

  // No copying allowed
  ConfigReader(const ConfigReader &);
  void operator=(const ConfigReader &);
};

}  // namespace yaseek

#endif  // UTILS_CONFIGS_H_
