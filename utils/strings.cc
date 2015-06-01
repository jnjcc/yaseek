// Copyright (c) 2012, 2014 Yste.org
// All Rights Reserved.

#include "strings.h"
#include <stdlib.h>

namespace yaseek {

int Split(const std::string &line, const std::string &sep,
    std::vector<std::string> *pvec) {
  int start = 0, end = 0;
  int lsze = line.size();
  int ssze = sep.size();
  while (start < lsze && ((end = line.find(sep, start)) != std::string::npos)) {
    // START might be equal to END
    pvec->push_back(line.substr(start, end - start));
    start = end + ssze;
  }
  if (start <= lsze) {
    pvec->push_back(line.substr(start, lsze - start));
  }
  return pvec->size();
}
int split(const char *line, const char *sep, char **items, int maxitems,
    int maxsize) {
  maxsize -= 1;
  if (maxsize <= 0 || line == NULL || sep == NULL || items == NULL) {
    return -1; 
  }
  int ssize = strlen(sep);
  int nitem = 0;
  const char *psep = NULL;
  while (nitem < maxitems && line && (psep = strstr(line, sep)) != NULL) {
    int isize = psep - line;
    // ISIZE might be zero
    if (isize > maxsize) {
      isize = maxsize;
    }   
    strncpy(items[nitem], line, isize);
    items[nitem][isize] = '\0';
    line = psep + ssize;
    nitem += 1;
  }
  if (nitem < maxitems && line) {
    strncpy(items[nitem], line, maxsize);
    items[nitem][maxsize] = '\0';
    ++nitem;
  }
  return nitem;
}
int split2d(const char *line, const char *sep, char *item2d, int maxitems,
    int maxsize) {
  char **items = (char **)malloc(maxitems * sizeof(char *));
  if (items == NULL) {
    return -1; 
  }
  for (int i = 0; i < maxitems; ++i) {
    items[i] = item2d + i * maxsize;
  }
  int nitem = split(line, sep, items, maxitems, maxsize);
  free(items);
  return nitem;
}

int tokenize(char *line, const char *delim, char **items, int maxitems) {
  char *tok = NULL;
  int nitem = 0;
  if (line == NULL || delim == NULL || items == NULL) {
    return 0;
  }
  while (nitem < maxitems && (tok = strtok(line, delim)) != NULL) {
    line = NULL;
    items[nitem] = tok;
    ++nitem;
  }
  return nitem;
}

static int isstrtype(const char *str, int (*ischrtype)(int)) {
  if (str == NULL) {
    return 0;
  }
  while (str && *str) {
    if (!ischrtype(*str)) {
      return 0;
    }   
    ++str;
  }
  return 1;
}
int isalnums(const char *str) {
  return isstrtype(str, isalnum);
}
int isalphas(const char *str) {
  return isstrtype(str, isalpha);
}
int isdigits(const char *str) {
  return isstrtype(str, isdigit);
}
int islowers(const char *str) {
  return isstrtype(str, islower);
}
int isspaces(const char *str) {
  return isstrtype(str, isspace);
}
int isuppers(const char *str) {
  return isstrtype(str, isupper);
}

static std::string &LTrimType(std::string &str, int (*ischrtype)(int)) {
  if (str.empty()) {
    return str;
  }
  std::string::iterator p = str.begin();
  while (p != str.end() && ischrtype(*p)) {
    ++p;
  }
  str.erase(str.begin(), p); 
  return str;
}
static std::string &RTrimType(std::string &str, int (*ischrtype)(int)) {
  if (str.empty()) {
    return str;
  }
  std::string::iterator p = str.end() - 1;
  while (p != str.begin() && ischrtype(*p)) {
    --p;
  }
  if (!ischrtype(*p)) {
    ++p;
  }
  str.erase(p, str.end());
  return str;
}
std::string &LTrims(std::string &str) {
  return LTrimType(str, isspace);
}
std::string &RTrims(std::string &str) {
  return RTrimType(str, isspace);
}
std::string &Trims(std::string &str) {
  return RTrims(LTrims(str));
}
std::string &TrimCmnt(std::string &str, int cmnt) {
  if (str.empty()) {
    return str;
  }
  std::string::iterator p = str.begin();
  while (p != str.end() && (*p != cmnt)) {
    ++p;
  }
  str.erase(p, str.end());
  return str;
}
static char *ltrimtype(const char *str, int (*ischrtype)(int)) {
  char *p = (char *)str;
  while (p && *p && ischrtype(*p)) {
    ++p;
  }
  return p;
}
static char *rtrimtype(char *str, int (*ischrtype)(int)) {
  if (str == NULL || *str == '\0') {
    return str;
  }
  char *p = str + strlen(str) - 1;
  while ((p != str) && ischrtype(*p)) {
    --p;
  }
  if (ischrtype(*p)) {  // p == line
    *p = '\0';
  } else {
    *(p + 1) = '\0';
  }
  return str;
}
char *ltrims(const char *str) {
  return ltrimtype(str, isspace);
}
char *rtrims(char *str) {
  return rtrimtype(str, isspace);
}
char *trims(char *str) {
  return rtrims(ltrims(str));
}
char *trimcmnt(char *str, int cmnt) {
  char *pcmnt = NULL;
  if (str && (pcmnt = strchr(str, cmnt)) != NULL) {
    *pcmnt = '\0';
  }
  return str;
}

}  // namespace yaseek
